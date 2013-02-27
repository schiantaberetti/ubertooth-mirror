LIBS = -lubertooth -lusb-1.0 -lbtbb -lbluetooth

all: 
	cc -o inquiry ubertooth-inq.c $(LIBS)
	cc -o inq-jam ubertooth-inq-jam.c $(LIBS)
	cc -o bt-jam ubertooth-bt-jam.c $(LIBS)

clean:
	rm -f inquiry
	rm -f inq-jam	
	rm -f bt-jam
	
