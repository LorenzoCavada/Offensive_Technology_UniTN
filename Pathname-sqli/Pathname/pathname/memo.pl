#!/usr/bin/perl -w

use CGI qw(:standard);
use CGI::Carp qw(fatalsToBrowser);
use strict;

# Barbazzo Fernap barbazzo@gue.com
# Gustar Woomax gustar@gue.com
# Wilbar Memboob wilbar@gue.com

# By the Frobozz Magic Software Company
# Released under the Grue Public License
# Suspendur 9th, 1068 GUE

# THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED
# BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE 
# COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM *AS IS* 
# WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, 
# BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
# FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY 
# AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE 
# DEFECTIVE, *AND IT WILL*, YOU ASSUME THE COST OF ALL NECESSARY 
# SERVICING, REPAIR OR CORRECTION.
	
my %labels; # global assoc. array (AA) of printable names for memos

# glob through the homedirs for an array of paths to memos sorted by date
# function list_memo_selector
# parameters: none
# output: fills global AA 'labels' with nice names for memos based on
# 	the pathname of the memo

sub list_memo_selector {
	
	# GET ALL THE USER MEMOS
	# the star '*' is a wildcard. This is a "glob" operator -- it finds
	# all the files in the 'memo' directory in any home directory located
	# in /home. E.g., it finds files in /home/foo/memo/ and /home/bar/memo
	# and stores them in a 0-indexed array (Perl arrays start with @).
	my @memos = </home/*/memo/*>; # all regular users

	# GET ROOT'S MEMOS
	# root can also have memos in /home/root/. The next glob operator
	# "pushes" root's memos onto the @memos array. This script (memo.cgi)
	# needs SUID-root permissions to access files in /root/memo.
	push (@memos, </root/memo/*>); # special memos from root


	my $memo; 
	foreach $memo (@memos) {
		# iterate through all memos listed in @memos. Each time
		# through this for-like loop, the memo's path will be stored
		# in the variable '$memo'. The 'foreach' loop will end after
		# the last memo is processed.

		# the following line is a "regular expression" which matches
		# the filename at the end of a path and stores the filename (only)
		# in the special variable "$1". E.g., if the path /foo/memo/i_quit
		# is in $memo, the string "i_quit" will be in $1 after the regular
		# expression.
		$memo =~ m#^.+/([^/]+)$#; # regex extract filename
		my $memoname = $1; # store the memoname part into $memoname
		$memoname =~ s/_/ /g; # turn memoname "i_quit" into "i quit"
		$labels{$memo} = $memoname; # assign pretty label name
		
		# now, %labels has an entry that looks like this:
		# $labels{'/foo/memo/i_quit'} pointing to the string "i quit"
	}

	# print an HTML "drop down menu" (called a popup_menu) with the name
	# 'memo' using the values of @memos as the choices and the pretty 
	# names in %labels (matching the @memos) as the readable names
	print popup_menu(-name=>'memo',
									 -values=>\@memos,
								   -labels=>\%labels);

	# print a submit button
	print submit("Read memo");

}

print header(); # print the header required for an HTML document
print "<html><head><title></title></head><body>\n";

print h1("FrobozzCo Memo Distribution Website");
print h4("Got Memo?");
print hr();

# start the HTML form and print the elements of it
print p('Select a memo from the popup menu below and click the "Read memo" button.');
print p("<form method='post' name='main'>\n");

if (!param('memo')) {
	# if no parameter named 'memo' was provided by the user, get a list
	# of all the memos on the system.
	list_memo_selector();
} else {
	# else, there is a memo parameter from the user. Figure out who wrote
	# the memo by looking at the path, and print the memo out.
	list_memo_selector();
	my $memo = param('memo');
	my $author = "root";
	my @stat = stat $memo;
	my $date = localtime $stat[9];
	if ($memo =~ m#^/home/([^/]+)/.*$#) {
		$author = $1;
	}
	print "<hr>\n";
	print "<blockquote>";
	print '<table border=1><tr><td>';
	print "<center><b>$labels{$memo}</b></center>";
	print '</td></tr>';
	print "<tr><td>\n<p>";
	print "<b>Author:</b> $author<br />\n";
	print "<b>Subject:</b> $labels{$memo}<br />";
	print "<b>Date:</b> $date<br />\n";
	print "\n</p></td></tr>\n";
	print "<tr><td><p>&nbsp;</p>\n";
	print "<blockquote><p>\n";
	
	open (MEMO, $memo); # open the memo file
	
	my $line;
	foreach $line (<MEMO>) {
		# for every line in the memo, print it out
		$line =~ s#\n$#</p><p>#; # replace newline characters with HTML
		print "$line\n";
	}
	print "</p></blockquote>\n";
	print '<p>&nbsp;</p></td></tr></table>';
	print "</blockquote>";
	print "<hr>\n";
}

# print some boilerplate instructions and quit
print h2("To publish a memo:");
print <<TEXT;

<ol>
<li>Create a directory named 'memo' in your home directory.</li>
<li>Edit text files in that directory.</li>
<li>Save the file using underscores (_) for spaces, e.g. "free_lunch".</li>
</ol>

TEXT

print p('To remove your memo from publication, simply delete the file from the memo directory.');

print "</form>\n";
print "</body></html>";
