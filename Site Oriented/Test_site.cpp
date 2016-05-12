//This is the simulation of TASEP for open boundary conditions using a 
//site oriented approach and for Random Sequential Update

#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

#define N 100		//To determine total size of circular track
#define P 0.5		//Probability of car moving
#define T_MAX 10000	//To determine number of points to plot
#define CHECK_POINT N/10 //Point where we check J
#define N_RUNS 100		//To define number of runs to take avg
#define FILE_NAME "results_beta_try28.dat"	//File to output data
#define FILE_NAME_CURRENT "current28.dat"	//File to output current
#define WARM_UP T_MAX/10	//Time for which warmup is assumed

float alpha = 0.2;		//Probability of a particle entering
float beta  = 0.8;		//Probability of a particle leaving

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

	int find(int t){

		for(int i = 0; i<q; i++){

			if(a[i] == t){
				return i;
			}
		}

		cout<<"Particle not found";

	}

};


int track[N];   //To maintain the circular track 
check_list list;		//To maintain where the 1's are
int count;			//For calculating J later
float flux[N];		//To calculate the net flux 


void initialize(int a[], int n) {

	for(int i = 0; i<n; i++) {

		a[i] = 0;
	}
}


void initialize(float a[], int n) {

	for(int i = 0; i<n; i++) {

		a[i] = 0;
	}
}

float rand1(){		//To generate values b/w 0,1

	float val = (float)rand()/(RAND_MAX);

	return val;
}


int ret_rand(int a){	//To return int b/w 0,a

	repeat:

	int val = a*rand1();

	if(val == a){
		goto repeat;
	}

	return val;

}

	
void print(int a[], int n){

	for(int i = 0; i< n; i++){
		cout<<a[i]<<" ";
	}

	cout<<"/"<<list.q<<endl;
}

void move(int time1){


	if(track[0] == 0 && rand1() < alpha){	//To see if particle added

		track[0] = 1;

		if(list.q>= 100){
			cout<<"here1"<<endl;
		}

		list.append(0);
		//list.display();
	}

	if(track[N-1] == 1 && rand1() < beta){

		track[N-1] = 0;  

		list.remove(N-1);
	}

	for(int i= 0; i<N; i++){

		int num = ret_rand(N);		//Number of list picked

		if(track[num] == 0){
			continue;
		}	

		num = list.find(num);		//To get the position in list array

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

					if(time1 >= WARM_UP){
						flux[pos]++;
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

	ofstream f1;
	ofstream f2;
	f1.open(FILE_NAME);		//To output density
	f2.open(FILE_NAME_CURRENT); 	//To output flux

	
	int current_avg = 0;

	float density_avg[N];

	initialize(density_avg, N);
	initialize(flux, N);

	for(long int runs = 0; runs< N_RUNS; runs++){
		
		initialize(track, N);
		list.initialize();


		for(int time1 = 0; time1 <T_MAX; time1++){

			//count = 0;
			move(time1);
/*
			if(runs == 10){
				print(track, N);
			}
*/
			if(time1 >= WARM_UP){
				
				for(int i= 0; i<N; i++){

					density_avg[i] += track[i];

				}
			}

		}

		cout<<(runs*100)/N_RUNS<<" %"<<endl;

	}

	for(long int i = 0; i<N; i++){

		density_avg[i] = (float)(density_avg[i])/(N_RUNS*(T_MAX- WARM_UP));
		flux[i] = (float)(flux[i])/(N_RUNS*(T_MAX- WARM_UP));


		f1<<(i+1)<<" "<<density_avg[i]<<endl;
		f2<<(i+1)<<" "<<flux[i]<<endl;			
	}

		//print(track);
/*
		if(list.q>= 100){
			list.display();
		}
*/

	f2.close();
	f1.close();

	//print(list);


	return 0;
}