1. enable Port Forwarding
	ssh -L 8118:server.pathname-cvd.offtech:80 otech2ak@users.deterlab.net
	
2. show normal behaviour of application

3. Show developer tool and option

4. Show post request

5. test the URL by inserting a path by hand

6. create a test file and try to access

7. try to access /etc/shadow

8. Go to /usr/lib/cgi-bin/

9. show privilage of each file (s stay for SUID)

10. remove the s
	sudo chmod u-s /usr/lib/cgi-bin/memo.cgi
	
11. try again accessing /etc/shadow

12. show that is still possible to reach the test file

13. show the fix abs_path and check

	badstring = /home/username/memo/../test

	abs_path(badstring)

	/home/username/test/
	
14. copy and appy the patch
	sudo cp ~/pathname-cvd/fixed.patch .
	
	print permission
	apply patch 
		patch bug_memo.pl -i fixed.patch -o memo.pl
	
	reset permission
		 chmod 655 memo.pl

15. test again the application


