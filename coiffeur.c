/* coiffeur.c */

#include <stdio.h>
#include <pthread.h>
/*****/ #include <stdlib.h>

#define NbTh 10      //Nombre de processus symbolisant les clients
#define N  8	//Nombre de chaises dans le salon de coiffure

pthread_t tid[NbTh+1];
pthread_mutex_t mutex;
pthread_cond_t attendre, dormir;

//initialisations
int NbClientsAttente=0;    //Nombre de clients en attente d'etre coiffes


void Coiffer ()
{
	
        if(NbClientsAttente>0) 
		{
		// dit à un client de s'installer sur le fauteuil
		/*****/pthread_mutex_lock(&mutex);
  	    	/*****/pthread_cond_wait(&attendre,&mutex);
  	    	/*****/pthread_mutex_unlock(&mutex);
		}
	
	else       {
		printf("le coiffeur dort car pas de clients \n");
               // dit à un client de s'installer sur le fauteuil
  	    	/*****/pthread_mutex_lock(&mutex);
  	    	/*****/pthread_cond_wait(&dormir,&mutex);
  	    	/*****/pthread_mutex_unlock(&mutex);
		}
	

}

void Client(int i)
{

 
	if(NbClientsAttente<N) 
	{
	
		// avertit le coiffeur qu'il est la et s'assoit
		/*****/pthread_cond_signal(&dormir);
		
		printf("Le client %d avertit le coiffeur qu'il est la et s'assoit \n",i);

		// on deverouille le mutex et attend que la condition soit signalée
		
		/*****/pthread_mutex_unlock(&mutex);
		
		
		// attend qu'on lui dise de s'installer
		
		/*****/pthread_cond_signal(&attendre);
		
		NbClientsAttente --;
		
		printf("Le coiffeur invite le client %d pour s'installer et le coiffer \n",(int)i);
		
	}
	else {
		printf("Le client %d ne trouve pas de place\n", i);
	}

}


void * fonc_coiffeur()
{                          

	while (1)  {
		Coiffer();
	/* temps de coiffure d'un client */
	usleep(200000);
   	}
}

void * fonc_client(void *i)
{
	Client((int)i);

	/* temps de coiffure */
	usleep(200000);
	printf("Le client %d quitte le salon\n", (int) i);
}


int main() 
{

	int num;


	// creation de la thread coiffeur
	pthread_create(tid+NbTh,0,(void *(*)())fonc_coiffeur,(void*)NbTh);

	//creation des threads clients
	for(num=0;num<NbTh;num ++)
        	pthread_create(tid+num,0,(void *(*)())fonc_client,(void*)num);

	//attend la fin de toutes les threads clients
	for(num=0;num<NbTh;num ++)
        	pthread_join(tid[num],NULL);
       
        
	/* liberation des ressources");*/
	
	
	/*****/ pthread_cond_destroy(&attendre);
	/*****/ pthread_cond_destroy(&dormir);
	
	exit(0);
}

