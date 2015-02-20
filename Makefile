all: TreeReader.run

TreeReader.run: TopTools/SF_ID-ISO-Trigger/SFIDISOTrigger.o TopTools/DY_DD/DYestimate.o TreeReader.o 
	gcc -o TreeReader.run TreeReader.o TopTools/SF_ID-ISO-Trigger/SFIDISOTrigger.o TopTools/DY_DD/DYestimate.o `root-config --libs`

TreeReader.o: TreeReader.C
	gcc -I`root-config --incdir` -ITopTools/SF_ID-ISO-Trigger -ITopTools/DY_DD -c -g TreeReader.C

TopTools/SF_ID-ISO-Trigger/SFIDISOTrigger.o: TopTools/SF_ID-ISO-Trigger/SFIDISOTrigger.C
	gcc -I `root-config --incdir` -c TopTools/SF_ID-ISO-Trigger/SFIDISOTrigger.C -o TopTools/SF_ID-ISO-Trigger/SFIDISOTrigger.o

TopTools/DY_DD/DYestimate.o: TopTools/DY_DD/DYestimate.C
	gcc -I `root-config --incdir` -c TopTools/DY_DD/DYestimate.C -o TopTools/DY_DD/DYestimate.o

clean:
	rm TreeReader.run TreeReader.o TopTools/SF_ID-ISO-Trigger/SFIDISOTrigger.o TopTools/DY_DD/DYestimate.o