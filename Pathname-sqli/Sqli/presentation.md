1. enable Port forwarding
	ssh -L 8118:server.sqli-cvd.offtech:80 otech2ak@users.deterlab.net
	
2. show normal login (221 XZa5haP)

3. perform first SQL Injection explaining the query structure

4. enter to id = 776 and empty the balance

5. explain why is impossible to execute multiple query
	2; UPDATE accounts SET bal=999 WHERE id=211;--
	
6. explain the fix

7. copy the patch to /usr/lib/cgi-bin/

8. change the name of FCCU.php and apply the patch

9. show again the service with the fail of the sqli

10. explain the other vulnerabilities

	Big number overflow
	use of name + surname as identifier
	bug in transfer founds
	use of clear password



