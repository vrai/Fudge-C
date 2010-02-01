# Copyright (C) 2009 - 2010, Vrai Stacey.
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
 
lib_LTLIBRARIES = libfudgec.la

INCLUDES = -I$(top_srcdir)/include

noinst_HEADERS = codec_decode.h         \
                 codec_encode.h         \
                 coerce.h               \
                 header.h               \
                 message_internal.h     \
                 prefix.h               \
                 pstdint.h              \
                 reference.h            \
                 registry_internal.h

libfudgec_la_SOURCES = codec_decode.c   \
                       codec_encode.c   \
                       coerce.c         \
                       fudge.c          \
                       header.c         \
                       message.c        \
                       message_ex.c     \
                       platform.c       \
                       prefix.c         \
                       reference.c      \
                       registry.c       \
                       status.c         \
                       types.c

libfudgec_la_LDFLAGS = -no-undefined -version-info @API_VERSION@

