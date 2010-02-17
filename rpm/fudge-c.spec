# Copyright 2009 by OpenGamma Inc and other contributors.
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

Name:		fudge-c
Version:	0.2
Release:	beta1%{?dist}
Summary:	Fudge message encoding library for C

Group:		Development/Libraries
License:	http://www.apache.org/licenses/LICENSE-2.0
URL:		http://www.fudgemsg.org/
Source:		%{name}-%{version}.tar.gz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

%description
Fudge is a hierarchical, typesafe, binary, self-describing message encoding system.

%prep
%setup -q

%build
./configure
make

%install
rm -rf $RPM_BUILD_ROOT
make install prefix=$RPM_BUILD_ROOT/usr

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(0644,root,root,0755)
/usr/include/fudge/*.h
/usr/lib/debug/usr/lib/libfudgec*
/usr/lib/libfudgec*
/usr/src/debug/%{name}-%{version}/*

%changelog
* Wed Feb 17 2010 Andrew Griffin <andrew@opengamma.com
- file created
