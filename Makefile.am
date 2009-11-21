# Copyright (C) 2009 - 2009, Vrai Stacey.
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
 
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = reconf configure
SUBDIRS = src include tests

DIST_SUBDIRS = src include tests

distclean-local:
	$(RM) -f _configs.sed

dist-hook: GenChangeLog

.PHONY: GenChangeLog
GenChangeLog:
	if test -d .git; then \
        $(top_srcdir)/scripts/git2changelog.py > $(distdir)/ChangeLog; \
    fi

