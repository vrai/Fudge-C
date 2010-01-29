# Copyright (C) 2010 - 2010, Vrai Stacey.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
#     
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
 
# By default assume that Fudge-C has been installed in the standard system
# location. If the headers and library are installed in directories that the
# compiler will not be able to find by default, add the relevant -I / -L
# arguments here.
INCLUDES=
LIBS=-lfudgec

TARGETS=simple prettyprint usertypes

.PHONY: all clean

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)
	$(RM) -r *.dSYM

$(TARGETS):
	$(CC) $(CFLAGS) -Wall -pedantic $(INCLUDES) $(LIBS) -o $@ $@.c
