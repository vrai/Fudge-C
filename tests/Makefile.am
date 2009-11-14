#  Copyright (C) 2009 - 2009, Vrai Stacey.
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
        test_message

check_PROGRAMS = $(TESTS)

INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/src

FRAMEWORK_SOURCE = simpletest.c

test_utilities_SOURCES = test_utilities.c $(FRAMEWORK_SOURCE)
test_utilities_LDADD = $(top_builddir)/src/libfudgec.la

test_message_SOURCES = test_message.c $(FRAMEWORK_SOURCE)
test_message_LDADD = $(top_builddir)/src/libfudgec.la

clean-local:
	$(RM) -f *.log

