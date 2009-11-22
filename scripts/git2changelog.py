#!/usr/bin/python -t

# Copyright (C) 2009 - 2009, Vrai Stacey.
#
# Part of the Fudge-C distribution.
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
 
import os, re, textwrap, sys

_commitReEx = re.compile ( '^commit\s+([0-9a-fA-F]+)' )
_authorReEx = re.compile ( '^Author:\s+(.+)$' )
_dateReEx = re.compile ( '^Date:\s+(\d{4}\-\d{2}\-\d{2})' )
_messageReEx = re.compile ( '^\s\s+.*$' )
_emptyLineReEx = re.compile ( '^$' )
_statReEx = re.compile ( '^\s+([^\|]+)\s+\|' )
_endStatReEx = re.compile ( '^\s+\d+\s+files' )
_fileModeReEx = re.compile ( '^\s+[a-z]+\s+mode\s+\d+\s+(.+)$' )
_initialWS = re.compile ( '^\s' )
_magicPrefix = re.compile ( '^\s*[(]+' )

class Change:
    def __init__ ( self, commit ):
        self.commit = commit
        self.author = None
        self.date = None
        self.message = ''
        self.allmessage = False
        self.files = [ ] 
        self.allstats = False

    def output ( self, output ):
        # Header goes first
        output.write ( '%s %s\n\n' % ( self.author, self.date ) )

        # Then the files
        if self.files:
            output.write ( '\n'.join ( [ '\t* %s' % file for file in sorted ( self.files ) ] ) )
            output.write ( ':\n' )

        # Remove common initial whitespace from the message lines and then
        # merge any lines that don't start with (non-common) whitespace to the
        # end of the previous line.
        lines = [ ]
        for line in textwrap.dedent ( self.message ).split ( '\n' ):
            if _initialWS.match ( line ) or not lines:
                lines.append ( line )
            else:
                lines [ len ( lines ) -1 ] = lines [ len ( lines ) -1 ] + ' ' + line

        # Wrap and indent the message - to breaking the ChangeLog format,
        # replace any special characters at the start of the line.
        for line in lines:
            for wrapped in textwrap.wrap ( line, 70 ):
                if _magicPrefix.match ( wrapped ):
                    wrapped = '_' + wrapped
                output.write ( '\t' + wrapped + '\n' )

        # Append the Git commit hash to message
        output.write ( '\t\n\tGit commit: %s\n\n' % self.commit )

def CheckForAuthor ( line, change ):
    if change.author:
        return False
    match = _authorReEx.match ( line )
    if match:
        change.author = match.group ( 1 ).strip ( )
    return match

def CheckForDate ( line, change ):
    if change.date:
        return False
    match = _dateReEx.match ( line )
    if match:
        change.date = match.group ( 1 )
    return match

def CheckForMessage ( line, change ):
    if change.allmessage:
        return False
    if change.message and _emptyLineReEx.match ( line ):
        change.allmessage = True
        return True
    match = _messageReEx.match ( line )
    if match:
        change.message += match.group ( 0 ) + '\n'
    return match

def CheckForStatFile ( line, change ):
    if change.allstats:
        return False
    match = _statReEx.match ( line )
    if match:
        change.files.append ( match.group ( 1 ).strip ( ) )
        return True
    elif _endStatReEx.match ( line ):
        change.allstats = True
        return True
    return False

def ConvertGitToChangeLog ( input, output ):
    change = None
    for line in input:
        # The "commit" tag signifies the start of a new change
        match = _commitReEx.match ( line )
        if match:
            if change:
                change.output ( output )
            change = Change ( match.group ( 1 ) )
            continue

        # The first line should have been a commit, as such their should be no
        # way to be here and there not be a change instance
        if not change:
            raise Exception, 'Found line outside of Change: %s' % line.strip ( )

        # The author and date should be next - will ignore anything else until
        # they are encountered
        if CheckForAuthor ( line, change ): continue
        if CheckForDate ( line, change ): continue
        if not ( change.author and change.date ):
            continue

        # Next up is the message, again don't continue until the message has
        # been completely read
        if CheckForMessage ( line, change ): continue
        if not change.allmessage:
            continue

        # Now the file change stats
        if CheckForStatFile ( line, change ): continue
        if not change.allstats:
            continue

        # Finally, get all of the mode operation (create, delete, etc) files
        match = _fileModeReEx.match ( line )
        if match:
            change.files.append ( match.group ( 1 ).strip ( ) )

    # Handle last change
    if change:
        change.output ( output )

if __name__ == '__main__':
    input = os.popen ( 'git log --summary --no-merges --no-color --stat --date=short', 'r' )
    output = sys.stdout
    try:
        ConvertGitToChangeLog ( input, output )
    finally:
        input.close ( )
        output.close ( )

