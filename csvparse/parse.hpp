
//          Copyright Ted Middleton 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_csvparse_parse_hpp
#define INCLUDED_csvparse_parse_hpp

#include <istream>
#include <iostream>
#include <vector>

namespace csvparse
{

template<int DELIMITER=',', bool ALLOW_LF=true>
class parser
{
    enum class eSTATE
    {
        START,
        DELIM,
        BODY,
        START_BODY_QUOTE,
        QUOTED_BODY,
        QUOTE,
        CR,
        LF,
        ERROR,
    };
public:
    parser()
        : m_state(eSTATE::START)
        , m_current(0)
    {
    }

    bool
    is_error() const
    {
        return m_state == eSTATE::ERROR;
    }

    template<typename END_FIELD, typename END_RECORD>
    bool
    advance(int ch, END_FIELD& end_field, END_RECORD& end_record)
    {
        std::cout << "Got character " << ch << 
            " (" << (char)ch << ") in state " << state() << "\n";
        switch (m_state) {
        case eSTATE::START:
        case eSTATE::DELIM:
        case eSTATE::LF:
            {
                switch (ch) {
                case DELIMITER:
                    m_state = eSTATE::DELIM;
                    end_field(m_current);
                    break;
                case '"':
                    m_state = eSTATE::START_BODY_QUOTE;
                    break;
                case '\r':
                    m_state = eSTATE::CR;
                    break;
                case '\n':
                    if constexpr (ALLOW_LF) {
                        m_state = eSTATE::LF;
                        end_record(m_current);
                    }
                    else {
                        m_state = eSTATE::ERROR;
                    }
                    break;
                default:
                    m_state = eSTATE::BODY;
                    break;
                }
            }
            break;
        case eSTATE::BODY:
            {
                switch (ch) {
                case DELIMITER:
                    m_state = eSTATE::DELIM;
                    end_field(m_current);
                    break;
                case '\r':
                    m_state = eSTATE::CR;
                    break;
                case '\n':
                    if constexpr (ALLOW_LF) {
                        m_state = eSTATE::LF;
                        end_record(m_current);
                    }
                    else {
                        m_state = eSTATE::ERROR;
                    }
                    break;
                case '"':
                    m_state = eSTATE::ERROR;
                    break;
                default:
                    break;
                }
            }
            break;
        case eSTATE::START_BODY_QUOTE:
        case eSTATE::QUOTED_BODY:
            {
                switch (ch) {
                case '"':
                    m_state = eSTATE::QUOTE;
                    break;
                default:
                    m_state = eSTATE::QUOTED_BODY;
                    break;
                }
            }
            break;
        case eSTATE::QUOTE:
            {
                switch (ch) {
                case DELIMITER:
                    m_state = eSTATE::DELIM;
                    end_field(m_current);
                    break;
                case '"':
                    m_state = eSTATE::QUOTED_BODY;
                    break;
                default:
                    m_state = eSTATE::ERROR;
                    break;
                }
            }
            break;
        case eSTATE::CR:
            {
                switch (ch) {
                case '\n':
                    m_state = eSTATE::LF;
                    end_record(m_current);
                    break;
                default:
                    m_state = eSTATE::ERROR;
                    break;
                }
            }
            break;
        case eSTATE::ERROR:
            break;
        }
        ++m_current;
        return !is_error();
    }

private:
    std::string
    state() const
    {
        switch (m_state) {
            case eSTATE::START:             return "START";
            case eSTATE::DELIM:             return "DELIM";
            case eSTATE::BODY:              return "BODY";
            case eSTATE::START_BODY_QUOTE:  return "START_BODY_QUOTE";
            case eSTATE::QUOTED_BODY:       return "QUOTED_BODY";
            case eSTATE::QUOTE:             return "QUOTE";
            case eSTATE::CR:                return "CR";
            case eSTATE::LF:                return "LF";
            case eSTATE::ERROR:             return "ERROR";
        }
        return "";
    }

    // +ve is a column break, -ve is a row/line break
    eSTATE m_state;
    size_t m_current;
};

} // namespace csvparse
 
#endif // INCLUDED_csvparse_parse_hpp
