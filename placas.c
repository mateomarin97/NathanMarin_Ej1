#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define L 5
#define lp 2
#define d 1
#define nfilas 256
#define V0 100
#define N 131072
#define w 1.0
#define p1i 26445
#define p1f 26547
#define p2i 39501
#define p2f 39603

int main(int argc, char **argv){
  int world_rank, world_size;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  /*
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
DECLARACION - DECLARACION - DECLARACION
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
  */

  int con;
  int con1;
  int con2;
  //procesadores jodidos porque les tocaron las placas
  int pj1;
  int pj2;
  int fpp;
  fpp = (int)nfilas/world_size;
  int pi;
  pi=fpp*nfilas*world_rank;
  int pf;
  pf = (fpp*nfilas*(world_rank+1))-1;
  float *vps;
  vps = malloc(nfilas*sizeof(float));
  float *vp;
  vp = malloc(nfilas*(fpp-2)*sizeof(float));
  float *vpi;
  vpi = malloc(nfilas*sizeof(float));
  float *ves;
  float *vei;
  //Lista donde cada procesador va a guardar todos su voltajes al final
  float *vpf;
  vpf = malloc(nfilas*fpp*sizeof(float));
  //Lista donde el cero va a guardar todos los voltajes al final
  float *vpt;



  if(world_size==4){
    //a pj1 le cayo en la mitad
    pj1 =1;
    //a pj2 le cayo en la mitad
    pj2 = 2 ;
  }
  else if(world_size==8){
    //a pj1 le cayo en la mitad
    pj1 = 3;
    //a pj2 le cayo en la mitad
    pj2 = 4;
  }
  else if(world_size==16){
    //a pj1 le cayo en la mitad
    pj1 = 6;
    //a pj2 le cayo en la mitad
    pj2 = 9;
  }
  else if(world_size==32){
    //a pj1 le cayo en la ultima linea
    pj1 = 12;
    //a pj2 le cayo en la mitad
    pj2 = 19;
  }
  else if(world_size==64){
    //a pj1 le cayo en la mitad
    pj1 = 25;
    //a pj2 le cayo en la mitad
    pj2 = 38;
  }

  if(world_rank==0){

    vei = malloc(nfilas*sizeof(float));
    vpt = malloc(nfilas*nfilas*sizeof(float));
  }
  else if(world_rank == (world_size-1)){

    ves = malloc(nfilas*sizeof(float));
  }
  else{
    ves = malloc(nfilas*sizeof(float));

    vei = malloc(nfilas*sizeof(float));
  }

  /*
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
INIZIALIZACION - INICIALIZACION - INICIALIZACION
-------------------------------------------------------------------
-------------------------------------------------------------------
-------------------------------------------------------------------
  */
  if(world_rank==0){
    //llenamos el primer procesador
    for(con=0;con<nfilas;con++){
      vps[con] = 0.0;
      vpi[con] = 0.0;
    }
    //el unico caso donde le puede tocar la placa
    if(world_size==2){

      for(con=0; con<(p1i - (nfilas));con++){
	       vp[con] = 0.0;
      }

      for(con=(p1i - (nfilas));con<(p1f-nfilas);con++){
	       vp[con] = V0/2.0;
      }

      for(con = (p1f-nfilas);con<nfilas*(fpp-2);con++){
	       vp[con] = 0.0;
      }

    }
    //en caso de que no le toquen las placas
    else{
      for(con=0;con<nfilas*(fpp-2);con++){
	       vp[con] = 0.0;
      }
    }

  }
  //Para el ultimo procesador
  else if(world_rank == (world_size-1)){
    for(con = 0; con<nfilas;con++){
      vps[con] = 0.0;
      vpi[con] = 0.0;
    }
    //en caso que le toque la placa
    if(world_size==2){
      for(con=0; con<(p2i - (nfilas+pi));con++){
	       vp[con] = 0.0;
      }

      for(con=(p2i - (nfilas+pi));con<(p2f-(nfilas+pi));con++){
	       vp[con] = -V0/2.0;
      }

      for(con = (p2f-(nfilas+pi));con<nfilas*(fpp-2);con++){
	       vp[con] = 0.0;
      }
    }
    //en caso que no le toque el procesador
    else{
      for(con=0;con<nfilas*(fpp-2);con++){
	       vp[con] = 0.0;
      }
    }

  }

  //los procesadores ensanduchados entre el primero y el ultimo
  else{
    //el primer procesador jodido
    if(world_rank==pj1 ){
      //Caso en que no caiga en la ultima promediable
      if(world_size!=32 && world_size!=64){
	//lenamos los extremos
	       for(con = 0; con<nfilas;con++){
	          vps[con]= 0.0;
	           vpi[con]= 0.0;
	          }
	//llenamos el sanduche de los sanduches
	       for(con = 0 ; con<(p1i-(pi+nfilas)) ; con++){
        	  vp[con] = 0.0;
	         }
	for(con =  (p1i-(pi+nfilas)); con<(p1f-(pi+nfilas)) ; con++){
	  vp[con] = V0/2.0;
	}
	for(con = (p1f-(pi+nfilas)); con<nfilas*(fpp-2) ; con++){
	  vp[con] = 0.0;
	}
      }

      else{
	//Lleno la superior
	for(con=0;con<nfilas;con++){
	  vps[con]=0.0;
	}
	//Lleno la del medio
	for(con=0;con<(nfilas*(fpp-2));con++){
	  vp[con]=0.0;
	}
	//Lleno la ultima
	for(con=0;con<(p1i-(pf-nfilas));con++){
	  vpi[con]=0.0;
	}
	for(con=(p1i-(pf-nfilas));con<(p1f-(pf-nfilas));con++){
	  vpi[con]= V0/2.0;
	}
	for(con=(p1f-(pf-nfilas));con<nfilas;con++){
	  vpi[con]= 0.0;
	}

      }
    }
    //el segundo procesador jodido
    else if(world_rank==pj2 ){
      //llenamos los extremos
      for(con = 0; con<nfilas;con++){
	vps[con]= 0.0;
	vpi[con]= 0.0;
      }
      //llenamos el sanduche de los sanduches
      for(con = 0 ; con<(p2i-(pi+nfilas)) ; con++){
	vp[con] = 0.0;
      }
      for(con =  (p2i-(pi+nfilas)); con<(p2f-(pi+nfilas)) ; con++){
	vp[con] = -V0/2.0;
      }
      for(con = (p2f-(pi+nfilas)); con<nfilas*(fpp-2) ; con++){
	vp[con] = 0.0;
      }

    }
    // en caso que no le toque ninguna placa
    else{
      for(con = 0; con<nfilas;con++){
	vps[con]= 0.0;
	vpi[con]= 0.0;
      }
      for(con=0;con<(nfilas*(fpp-2));con++){
	vp[con] = 0.0;
      }

    }
  }



  /*
--------------------------------------------------------------
--------------------------------------------------------------
FIN DE LA INIZIALIZACION BASICA, FALTA COMUNICAR FRONTERAS PARA CALCULAR
--------------------------------------------------------------
--------------------------------------------------------------
  */


  /*
--------------------------------------------------------------
--------------------------------------------------------------
FASE DE COMUNICACION Y PROMEDIO
--------------------------------------------------------------
--------------------------------------------------------------
  */


  for(con1=0;con1<N;con1++){

    /*
      ---------------------------------------------------------------------------------
      FASE COMUNICACION
      ---------------------------------------------------------------------------------
    */
    //Se comunica el cero
    if(world_rank==0){
      MPI_Send(vpi,nfilas,MPI_FLOAT,1,0,MPI_COMM_WORLD);
      MPI_Recv(vei,nfilas,MPI_FLOAT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    }
    //Se comunica el ultimo
    else if(world_rank==world_size-1){
      MPI_Send(vps,nfilas,MPI_FLOAT,world_size-2,0,MPI_COMM_WORLD);
      MPI_Recv(ves,nfilas,MPI_FLOAT,world_size-2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);


    }
    else{
      MPI_Send(vps,nfilas,MPI_FLOAT,world_rank-1,0,MPI_COMM_WORLD);
      MPI_Recv(vei,nfilas,MPI_FLOAT,world_rank+1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      MPI_Send(vpi,nfilas,MPI_FLOAT,world_rank+1,0,MPI_COMM_WORLD);
      MPI_Recv(ves,nfilas,MPI_FLOAT,world_rank-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }


    MPI_Barrier(MPI_COMM_WORLD);

    /*
      ---------------------------------------------------------------------------------------
      FASE DE PROMEDIO
      ---------------------------------------------------------------------------------------
    */
    //primer procesador
    if(world_rank==0){
      /*
      ----------------------------------------------------------------------------
      VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS
      ----------------------------------------------------------------------------
      */
      //primera fila
      vps[0] = (1-w)*vps[0] + (w/4.0)*(vps[1]+vp[0]);
      for(con = 1 ; con<(nfilas-1);con++){
        vps[con] = (1-w)*vps[con] + (w/4.0)*(vps[con-1]+vps[con+1]+vp[con]);
      }
      vps[nfilas-1] = (1-w)*vps[nfilas-1] + (w/4.0)*(vps[nfilas-2]+vp[nfilas-1]);



      /*
      ----------------------------------------------------------------------------
      VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP
      ----------------------------------------------------------------------------
      */
      //la primera casilla
      vp[0] = (1-w)*vp[0] + (w/4.0)*(vp[1]+vp[nfilas]+vps[0]);
      //itermos sobre vp
      for(con = 1 ; con<((nfilas*(fpp-2))-1);con++){
        //nos despreocupamos con un excelente machete las placas
        if(vp[con]!=50.0 && vp[con]!=-50.0 ){

          //puntos no problematicos (esquinas)
          if( con!=(nfilas-1) && con!=((nfilas*(fpp-2)-1)-nfilas+1)){
            //fila superior
            if( con>0 && con< (nfilas-1) ){
              vp[con]= (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vps[con]+vp[con+nfilas]);
            }
            //columna izq
            else if( con%nfilas == 0 ){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-nfilas]+vp[con+1]+vp[con+nfilas]);
            }
            //columan der
            else if(con%nfilas == (nfilas-1)){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-1]+vp[con-nfilas]+vp[con+nfilas]);
            }
            //fila inferior
            else if(con>((nfilas*(fpp-2)-1)-nfilas+1)){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vp[con-nfilas]+vpi[con%nfilas]);
            }
            //los puntos adentro
            else{
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vp[con-nfilas]+vp[con+nfilas]);
            }

          }
          else if(con==(nfilas-1)){
            vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-1]+vps[con]+vp[con+nfilas]);
          }
          else if(con==((nfilas*(fpp-2)-1)-nfilas+1)){
            vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vpi[con%nfilas]+vp[con-nfilas]);
          }



        }
      }

      vp[nfilas*(fpp-2)-1] = (1-w)*vp[nfilas*(fpp-2)-1] + (w/4.0)*(vp[nfilas*(fpp-2)-2]+vp[(nfilas*(fpp-2)-1)-nfilas]+vpi[nfilas-1]);

      /*
      ----------------------------------------------------------------------------
      VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI
      ----------------------------------------------------------------------------
      */

      vpi[0] = (1-w)*vpi[0] + (w/4.0)*(vpi[1]+vp[((nfilas*(fpp-2)-1)-nfilas+1)] + vei[0]);
      for(con = 1 ; con<(nfilas-1);con++){
        vpi[con] = (1-w)*vpi[con] + (w/4.0)*(vpi[con-1]+vpi[con+1]+vp[con+((nfilas*(fpp-2)-1)-nfilas+1)]+vei[con]);
      }
      vpi[nfilas-1] = (1-w)*vpi[nfilas-1] + (w/4.0)*(vpi[nfilas-2]+vp[nfilas*(fpp-2)-1]+vei[nfilas-1]);




    }



    //ultimo procesador


    else if(world_rank == (world_size-1)){

      /*
      ----------------------------------------------------------------------------
      VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS
      ----------------------------------------------------------------------------
      */
      //primera fila
      vps[0] = (1-w)*vps[0] + (w/4.0)*(vps[1]+vp[0]+ves[0]);
      for(con = 1 ; con<(nfilas-1);con++){
        vps[con] = (1-w)*vps[con] + (w/4.0)*(vps[con-1]+vps[con+1]+vp[con]+ves[con]);
      }
      vps[nfilas-1] = (1-w)*vps[nfilas-1] + (w/4.0)*(vps[nfilas-2]+vp[nfilas-1] + ves[nfilas-1]);

      /*
      ----------------------------------------------------------------------------
      VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP
      ----------------------------------------------------------------------------
      */
      //la primera casilla
      vp[0] = (1-w)*vp[0] + (w/4.0)*(vp[1]+vp[nfilas]+vps[0]);
      //itermos sobre vp
      for(con = 1 ; con<((nfilas*(fpp-2))-1);con++){
        //nos despreocupamos con un excelente machete las placas
        if(vp[con]!=50.0 && vp[con]!=-50.0 ){

          //puntos no problematicos (esquinas)
          if( con!=(nfilas-1) && con!=((nfilas*(fpp-2)-1)-nfilas+1)){
            //fila superior
            if(con>0&&con<nfilas-1){
              vp[con]= (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vps[con]+vp[con+nfilas]);
            }
            //columna izq
            else if(con%nfilas==0){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-nfilas]+vp[con+1]+vp[con+nfilas]);
            }
            //columan der
            else if(con%nfilas == (nfilas-1)){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-1]+vp[con-nfilas]+vp[con+nfilas]);
            }
            //fila inferior
            else if(con>((nfilas*(fpp-2)-1)-nfilas+1)){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vp[con-nfilas]+vpi[con%nfilas]);
            }
            //los puntos adentro
            else{
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vp[con-nfilas]+vp[con+nfilas]);
            }

          }
          else if(con==(nfilas-1)){
            vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-1]+vps[con]+vp[con+nfilas]);
          }
          else if(con==((nfilas*(fpp-2)-1)-nfilas+1)){
            vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vpi[con%nfilas]+vp[con-nfilas]);
          }



        }
      }

      vp[nfilas*(fpp-2)-1] = (1-w)*vp[nfilas*(fpp-2)-1] + (w/4.0)*(vp[nfilas*(fpp-2)-2]+vp[((nfilas)*(fpp-2)-1)-nfilas]+vpi[nfilas-1]);

      /*
      ----------------------------------------------------------------------------
      VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI
      ----------------------------------------------------------------------------
      */

      vpi[0] = (1-w)*vpi[0] + (w/4.0)*(vpi[1]+vp[((nfilas*(fpp-2)-1)-nfilas+1)]);
      for(con = 1 ; con<(nfilas-1);con++){
        vpi[con] = (1-w)*vpi[con] + (w/4.0)*(vpi[con-1]+vpi[con+1]+vp[con+((nfilas*(fpp-2)-1)-nfilas+1)]);
      }
      vpi[nfilas-1] = (1-w)*vpi[nfilas-1] + (w/4.0)*(vpi[nfilas-2]+vp[nfilas*(fpp-2)-1]);



    }
    //procesadores ensanduchados
    else{
      /*
      ----------------------------------------------------------------------------
      VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS - VPS
      ----------------------------------------------------------------------------
      */
      //primera fila
      vps[0] = (1-w)*vps[0] + (w/4.0)*(vps[1]+vp[0]+ves[0]);
      for(con = 1 ; con<(nfilas-1);con++){
        vps[con] = (1-w)*vps[con] + (w/4.0)*(vps[con-1]+vps[con+1]+vp[con]+ves[con]);
      }
      vps[nfilas-1] = (1-w)*vps[nfilas-1] + (w/4.0)*(vps[nfilas-2]+vp[nfilas-1] + ves[nfilas-1]);

      /*
      ----------------------------------------------------------------------------
      VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP-VP -VP
      ----------------------------------------------------------------------------
      */
      //la primera casilla
      vp[0] = (1-w)*vp[0] + (w/4.0)*(vp[1]+vp[nfilas]+vps[0]);
      //itermos sobre vp
      for(con = 1 ; con<((nfilas*(fpp-2))-1);con++){
        //nos despreocupamos con un excelente machete las placas
        if(vp[con]!=50.0 && vp[con]!=-50.0 ){

          //puntos no problematicos (esquinas)
          if( con!=(nfilas-1) && con!=((nfilas*(fpp-2)-1)-nfilas+1)){
            //fila superior
            if(con>0&&con<nfilas-1){
              vp[con]= (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vps[con]+vp[con+nfilas]);
            }
            //columna izq
            else if(con%nfilas==0){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-nfilas]+vp[con+1]+vp[con+nfilas]);
            }
            //columan der
            else if(con%nfilas == (nfilas-1)){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-1]+vp[con-nfilas]+vp[con+nfilas]);
            }
            //fila inferior
            else if(con>((nfilas*(fpp-2)-1)-nfilas+1)){
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vp[con-nfilas]+vpi[con%nfilas]);
            }
            //los puntos adentro
            else{
              vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vp[con-1]+vp[con-nfilas]+vp[con+nfilas]);
            }

          }
          else if(con==(nfilas-1)){
            vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con-1]+vps[con]+vp[con+nfilas]);
          }
          else if(con==((nfilas*(fpp-2)-1)-nfilas+1)){
            vp[con] = (1-w)*vp[con] + (w/4.0)*(vp[con+1]+vpi[con%nfilas]+vp[con-nfilas]);
          }



        }
      }

      vp[nfilas*(fpp-2)-1] = (1-w)*vp[nfilas*(fpp-2)-1] + (w/4.0)*(vp[nfilas*(fpp-2)-2]+vp[((nfilas)*(fpp-2)-1)-nfilas]+vpi[nfilas-1]);

      /*
      ----------------------------------------------------------------------------
      VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI - VPI
      ----------------------------------------------------------------------------
      */

      vpi[0] = (1-w)*vpi[0] + (w/4.0)*(vpi[1]+vp[((nfilas*(fpp-2)-1)-nfilas+1)] + vei[0]);
      for(con = 1 ; con<(nfilas-1);con++){
        if(vpi[con]!=50.0 && vpi[con]!=-50.0 ){
          vpi[con] = (1-w)*vpi[con] + (w/4.0)*(vpi[con-1]+vpi[con+1]+vp[con+((nfilas*(fpp-2)-1)-nfilas+1)]+vei[con]);
        }
      }
      vpi[nfilas-1] = (1-w)*vpi[nfilas-1] + (w/4.0)*(vpi[nfilas-2]+vp[nfilas*(fpp-2)-1]+vei[nfilas-1]);



    }

    MPI_Barrier(MPI_COMM_WORLD);
  }

  /*
  //IMRIME LAS LISTAS EXTREMAS - REVISAR BUENA COMUNICACION
  for(con1=0;con1<world_size;con1++){
    if(world_rank==0){
      for(con=0;con<nfilas;con++){
	printf("%f ",vei[con]);
      }
      printf("\n");
    }

   else if(world_rank==world_size-1){
      for(con=0;con<nfilas;con++){
	printf("%f ",ves[con]);
      }
      printf("\n");
    }
   else if(world_rank==con1){
     for(con=0;con<nfilas;con++){
       printf("%f ",ves[con]);
     }
     printf("\n");
     for(con=0;con<nfilas;con++){
       printf("%f ",vei[con]);
     }
     printf("\n");

   }
    MPI_Barrier(MPI_COMM_WORLD);

  }*/


  /*


-------------------------------------------------------------
-------------------------------------------------------------
Llena vpf y posteriormente comunica todo al cero
-------------------------------------------------------------
-------------------------------------------------------------
  */
  for(con1=0;con1<world_size;con1++){
    if(world_rank==con1){
      for(con=0;con<(fpp*nfilas);con++){
	if(con<nfilas){
	  vpf[con]=vps[con];
	}
	else if(con>=nfilas && con<(nfilas+(fpp-2)*nfilas)){
	  vpf[con]=vp[con];
	}
	else {
	  vpf[con]=vpi[con];
	}
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);

  }

  //Finalmente le comunico todo al cero

  MPI_Gather(vpf,nfilas*fpp,MPI_FLOAT,vpt,nfilas*fpp,MPI_FLOAT,0,MPI_COMM_WORLD);

  /*
--------------------------------------------------------------
--------------------------------------------------------------
 IMPRIMIR EL ESTADO ACTUAL DEL GRID
--------------------------------------------------------------
--------------------------------------------------------------
  */
  if(world_rank==0){
    for(con=0;con<nfilas*nfilas;con++){
      printf("%f ",vpt[con]);
      if(con%nfilas==(nfilas-1)){
	printf("\n");
      }

    }

  }




  /*  MPI_Barrier(MPI_COMM_WORLD);
  //iteramos sobre los procesadores
    for(con=0;con<world_size;con++){
      MPI_Barrier(MPI_COMM_WORLD);
     if(world_rank==con){
      //cada proc imprime los suyo

      // //la primera linea
       for(con1 = 0 ; con1<nfilas;con1++){
        fprintf(stderr," %f ,",vps[con1] );

      }
       // printf("\n");

      //
      //
      // //las lineas del sanduche
      for(con1 = 0 ; con1<(fpp-2);con1++){
          for(con2 = 0 ; con2 < nfilas ; con2++){
              fprintf(stderr," %f ,",vp[con1*nfilas +con2] );
          }
	  //   printf("\n");

      }
      //
      //

      //la ultima linea
       for(con1 = 0 ; con1 < nfilas ; con1++){
         fprintf(stderr," %f ,",vpi[con1] );
	 //  if(con1%(nfilas)==(nfilas-1)){
      //     printf("\n" );
      //   }
       }



    }
    MPI_Barrier(MPI_COMM_WORLD);



  }
  */

  // printf("%d",world_size);



  MPI_Finalize();
  return 0;
}
