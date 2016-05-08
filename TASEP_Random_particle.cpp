//This is the simulation of TASEP for open boundary conditions using a 
//particle oriented approach and for Random Sequential Update

#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

#define N 100		//To determine total size of circular track
#define P 0.5		//Probability of car moving
#define T_MAX 100	//To determine number of points to plot
#define CHECK_POINT N/10 //Point where we check J
#define N_RUNS 10000		//To define number of runs to take avg
#define FILE_NAME "results_beta2_2.dat"	//File to output data
#define FILE_NAME_CURRENT "current3.dat"	//File to output current
#define WARM_UP N_RUNS/10	//Time for which warmup is assumed

float alpha = 0;		//Probability of a particle entering
float beta  = 0.3;		//Probability of a particle leaving

struct check_list{

	int a[N];					//to store the ones that have changed
	int q;						//to append

	check_list(){
		
		q = 0; 


		for(int i = 0; i<N; i++){

			a[i] = -1;
		}
	}

	void initialize(){

		q = 0; 


		for(int i = 0; i<N; i++){

			a[i] = -1;
		}

	}

	void append(int t){
		
		a[q] = t;
		q++; 
	}

	void remove(int t){

		int pos;

		for(int i = 0; i<q; i++){
			
			if( a[i] == t){
				pos = i; break;
			}
		}

		q--;

		for(int i = pos; i < q ; i++){

			a[i] = a[i+1];
		}


	}

	void display(){

		cout<<"///";

		for(int i= 0; i<q; i++){
			cout<<a[i]<<" ";
		}

		cout<<"///"<<endl;
	}

	int check(int t){

		for(int i = 0; i<q; i++){

			if(a[i] == t){
				return 1;
			}

		}

		return 0;
	}

};


int track[N];   //To maintain the circular track 
check_list list;		//To maintain where the 1's are
int count;			//For calculating J later


void initialize(int a[]) {

	for(int i = 0; i<N; i++) {

		a[i] = 0;
	}
}

float rand1(){		//To generate values b/w 0,1

	float val = (float)rand()/RAND_MAX;

	return val;
}


int ret_rand(int a){	//To return int b/w 0,a

	int val = a*rand1();

	return val;

}

	
void print(int a[]){

	for(int i = 0; i< N; i++){
		cout<<a[i]<<" ";
	}

	cout<<"/"<<list.q<<endl;
}

void move(int time){

	if(track[0] == 0 && rand1() < alpha){	//To see if particle added

		track[0] = 1;

		if(list.q>= 100){
			cout<<"here1"<<endl;
		}

		list.append(0);
		//list.display();
	}

	if(track[N-1] == 1 && rand1() <beta){

		track[N-1] = 0;

		list.remove(N-1);
	}

/*
	if(time == 58){		//To check if working
		list.display();
		print(track);
		count++;
	}	
*/

	for(int i= 0; i<list.q; i++){

		int num = ret_rand(list.q);		//Number of list picked

		int t = list.a[num];	//Position from track seen

		if(t != N-1){			//Any other case for a particle

			int pos = t;
			int next_pos = t+1;


			if(track[next_pos] == 0){

				float prob_check = rand1();
			
				if( prob_check < P){

					if(pos == CHECK_POINT){
						count++;
					}

					track[pos] =  0;
					track[next_pos] = 1;

					list.a[num] = next_pos;

				}

			}


		}	


	}
}


int main(){
	
	
	ofstream f1, f2;
	f1.open(FILE_NAME);		//To output density
	f2.open(FILE_NAME_CURRENT); 	//To output current



	for(int moves = 1; moves <= T_MAX; moves++){	//How many moves to see

		//srand(moves);
		int current_avg = 0;

		initialize(track);
		list.initialize();

		long int density_avg = 0;

		for(long int runs = 0; runs< N_RUNS; runs++){
			
			count = 0;
			move(moves);

			if( runs >= WARM_UP){
				density_avg += list.q;
				current_avg += count;
			}
		}

		//print(track);
/*
		if(list.q>= 100){
			list.display();
		}
*/
		f1<<alpha<<" "<<(float)density_avg/(N_RUNS - WARM_UP)/N<<endl;
		f2<<alpha<<" "<<(float)current_avg/(N_RUNS - WARM_UP)<<endl;

		if(moves%(T_MAX/100) == 0){
			cout<<moves*100/T_MAX<<" %"<<endl;
		}

		alpha += 1.0/T_MAX;
	}


	f2.close();
	f1.close();

	//print(list);


	return 0;
}