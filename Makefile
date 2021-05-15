dns_svr:
	gcc -o dns_svr main.c handler.c dataStructure.c parser.c -Wall

clean:
	rm -f dns_svr dns_svr.log
