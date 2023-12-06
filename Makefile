all: 
	
	gcc FICM.c -g -o FICM	
	gcc ITCM_sender.c -g -o ICTM
	gcc ECS.c -g -o ECS
	gcc SDF.c -g -o SDF
	gcc FAULT.c -g -o FAULT
	
final:
	gcc ECU.c -g -o ECU
	
rm:
	rm FICM ICTM ECS SDF FAULT ECU
