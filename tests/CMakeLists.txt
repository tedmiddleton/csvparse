
#          Copyright Ted Middleton 2022.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.0 )

add_executable( csvparse_test
    csvparse_test_main.cpp
    )

target_link_libraries( csvparse_test
    PRIVATE
        csvparse
    )

add_test( csvparse_test csvparse_test )

