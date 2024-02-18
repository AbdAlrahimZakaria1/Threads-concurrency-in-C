#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


int zombie_counter = 0;
int zombie_killed = 0;
int enteredZombies = 0;
#define MAX_ZOMBIE_NUMBER 100
int Chance50Percent = 0;

/*
Holds the count of how many zombies that entered the room. (Doesnt matter if the zombies are killed afterwards)
*/
void zombieEntered(){
	enteredZombies ++;
    zombie_counter += enteredZombies;
}

/*
@returns true if the zombies in the room count is more than or equal to 100, returns false otherwise 
*/
int tooManyZombiesInTheRoom(){ 
	if(zombie_counter >= 100){
		printf("number of zombies in the room are greater than or equal to 100\n");
		return 1;
	}
	else{
		printf("number of zombie in the room are less than 100\n");
		return 0;
	}
}


/*
increments the counter of zombies killed 
*/
void zombieKilled(){
	if(enteredZombies > zombie_counter){
		zombie_killed = zombie_counter - enteredZombies;
	}
	zombie_counter -= zombie_killed;
}


/*
@returns 1 if there has been more than 100 zombies killed, otherwise It returns 0
*/
int killed100Zombies(){
	if(zombie_killed > 100){
		printf("more than 100 zombies have been killed\n");
		return 1;
	}
	else{
		return 0;
	}
}


/*
@returns the number of zombies that have been killed.
*/
int getKilledCount(){
	void zombieKilled();
	return zombie_killed;
}


/*
@returns 1 if there is at least one zombie that exists, otherwise It returns 0
*/
int zombiesExist(){
	if(zombie_counter == 0)
		return 1;
	else
		return 0;
}


/*
@returns the number of zombies that are currently in the room.
*/
int getInTheRoomCount(){
	return zombie_counter;
}

void *slayer(void *p){
	pthread_t tid = (pthread_t)p;
	if(tid != 0){
		pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
		usleep(2);
		if(zombiesExist() ==  1){
			pthread_mutex_lock(&m1);
			int k = getKilledCount();
			k++;
			pthread_mutex_unlock(&m1);
		}
		if(getInTheRoomCount() > MAX_ZOMBIE_NUMBER || getKilledCount() > MAX_ZOMBIE_NUMBER){
				pthread_exit(NULL);
		}
	}
	else{
		printf("there is no slayer\n");
		pthread_exit(NULL);
	}
	return NULL;
}


void *doorMen(void *p){
	pthread_t door_man = (pthread_t)p;
	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	if(p != 0){
		usleep(3);
		/*
		each doorman goes into the loop twice, once to allow the zombie to enter, and once do not let the zombie ente.
		*/
		for(int i = 0; i < 2; i++){
			if(Chance50Percent % 2 == 0){
						pthread_mutex_lock(&m);
						zombieEntered();
						pthread_mutex_unlock(&m);
			}
			else{
						Chance50Percent ++;
			}
		}

	}
	else{
		printf("No DOOR man available\n");
	}
	if(getInTheRoomCount() > MAX_ZOMBIE_NUMBER || getKilledCount() > MAX_ZOMBIE_NUMBER){
		pthread_exit(NULL);
	}
	return NULL;
}


int main(int argc, char *argv[]){
	pthread_t thread_id2;
	pthread_t thread_id1;
	int rc1, n, rc2;
	rc1 = pthread_create(&thread_id1, NULL, slayer, (void*)1); //for slayer
	if(rc1){
		printf("\n return code from pthread_create is %d \n", rc1);
	    exit(1);
    }
	else{
		printf("Create a new thread for slayer\n");
	}

	if(argc < 2)
		 printf("Please add the number of threads to the command line\n");

	n = atoi(argv[2]);
	for(int i = 0; i < n; i++){
		rc2 = pthread_create(&thread_id2, NULL, doorMen, (void*)&n);
		if(rc2){
			printf("\n ERROR: return code from pthread_create is %d \n", rc2);
			exit(1);
	   }
	   else
			printf("\nCreating %d thread for DOORMAN\n", i + 1);
	}

	pthread_join(thread_id1, NULL);

	for(int i = 1; i < n+1; i++){
		pthread_join(thread_id2, NULL);
	}

	if(tooManyZombiesInTheRoom() == 1){
		printf("you been killed by the zombie!!\n");
	}
	if(killed100Zombies() == 1){
		printf("you killed 100 zombie!!\n");
	}

    pthread_exit(NULL);
}