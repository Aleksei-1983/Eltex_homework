/*Программа  генерирует 4 потока 1 это доставка 3 покупателя , 
у покупателя есть потребности в товарах они ходят в 1 из четырех 
магазинов случайным образом и берут от в нем весь товар  если он меньше их потребностей 
и уменьшают на эту величину свои потребности   
а погрузчик каждый раз привозит по 100 единиц в случайный магазин. */

#include "main.h"

int customerNeeds[NUMBER_UNITS - 1] = {1000, 1000, 1000};
int shops[NUMBER_SHOP] = {100, 100, 100, 100};


/*  Действие покупателя */
void* buyerActivity(void* arg){
  
  int *buyer = (int*) arg;
  int shop;
  int temp;
  int numberBuyer = 0;

  
  if(buyer == customerNeeds) numberBuyer = 1;
  if(buyer == (customerNeeds + 1)) numberBuyer = 2;
  if(buyer == (customerNeeds + 2)) numberBuyer = 3; 
   
  while(*buyer > 0){
    shop = rand() % 4;
    
    pthread_mutex_lock(&m1);
    
    if (shops[shop] == 0){
      pthread_mutex_unlock(&m1);
      continue;

    }
    if(*buyer >= shops[shop]){
      *buyer -= shops[shop];
      temp = shops[shop];
      shops[shop] = 0;
      
    }
    else{
      temp = *buyer;
      shops[shop] -= *buyer;
      *buyer = 0;
    }
    pthread_mutex_unlock(&m1);

    
    printf("Buyer %d took from shop %d, %d products, and need - %d \n",numberBuyer,  shop, temp, *buyer);
    sleep(1);
    
  }
    
    printf("Buyer %d has no need!!!\n", numberBuyer);
    
    return NULL;
  
}

/*  Действие доставщика */
void* deliverymanActivity(){
  
  int index = NUMBER_OF_SHIPMENTS;

  while(0 != index-- ){
    int shop = rand() % 4;
    pthread_mutex_lock(&m1);
    
    if(shops[shop] < 1000){
      shops[shop] += 100;
      
    pthread_mutex_unlock(&m1);  
      printf("Beliveryman brought 100 items to the %d shop\n", shop + 1);
    }
    sleep(2);
  }
  
}

int main (int argc, char** argv)
{

  int *state;
  int index;

  pthread_t thread[NUMBER_UNITS];

  printf("Bign \n\n");

  for(index = 0; index < NUMBER_UNITS; index++){
    if(index < NUMBER_UNITS - 1){
      pthread_create(&thread[index], NULL, buyerActivity, customerNeeds + index);
      continue;
    }
    pthread_create(&thread[index], NULL, deliverymanActivity, NULL);
  }


  for(index = 0; index < NUMBER_UNITS -1; index++){
    pthread_join(thread[index], (void**) &state);
  }
  
  printf("\n================\n");
  
  for(index = 0; index < NUMBER_SHOP; index++){
    printf("Shop %d items %d\n", index, shops[index]);
  }
  return 0;
}
