placas.pdf : grafica.py datos.dat
	python grafica.py
datos.dat : placas.x submit_job.sh
	qsub submit_job.sh
	mv out.o* datos.dat
placas.x : placas.c
	mpicc -o placas.x placas.c
clean :
	rm placas.pdf datos.dat placas.x out.e*
