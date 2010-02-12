#  Copyright (C) 2009 - 2010, Vrai Stacey.
#  
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#      http://www.apache.org/licenses/LICENSE-2.0
#      
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

TESTS = test_utilities  \
        test_message    \
        test_codec      \
        test_string     \
        test_user_types

check_PROGRAMS = $(TESTS)

noinst_HEADERS = ansi_compat.h  \
                 simpletest.h   \
                 snprintf.h

INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/src

FRAMEWORK_SOURCE = simpletest.c
EXTRA_DIST = test_data

test_utilities_SOURCES = test_utilities.c $(FRAMEWORK_SOURCE)
test_utilities_LDADD = $(top_builddir)/src/libfudgec.la

test_message_SOURCES = test_message.c $(FRAMEWORK_SOURCE)
test_message_LDADD = $(top_builddir)/src/libfudgec.la

test_codec_SOURCES = test_codec.c $(FRAMEWORK_SOURCE)
test_codec_LDADD = $(top_builddir)/src/libfudgec.la

test_string_SOURCES = test_string.c $(FRAMEWORK_SOURCE)
test_string_LDADD = $(top_builddir)/src/libfudgec.la

test_user_types_SOURCES = test_user_types.c $(FRAMEWORK_SOURCE)
test_user_types_LDADD = $(top_builddir)/src/libfudgec.la

clean-local:
	$(RM) -f *.log

