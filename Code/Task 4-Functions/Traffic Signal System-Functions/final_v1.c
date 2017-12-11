


/*functions started after  dotted line*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#define MAX_VERTICES 10
#define MAX_EDGES    MAX_VERTICES*(MAX_VERTICES-1)/2
#define MAX_TSJ 10
#define MAX_STREET 10

typedef struct coordinate
{
  int   x; //integral numeral for x-coordinate of the object
  int   y; //integral numeral for y-coordinate of the object
  float rotation; //floating point value for Angle of Rotation of the object
}coordinate_t;

typedef struct style
{
 int fill_color; //contains the color code of the graph
 int border_style; //contains the border style of the graph
 int border_color; //contains the color code of the graph
 int width; //contains the width of a lane/street or the radius of a circle
}style_t;

// Structure to represent each node which is used to extend the graph of TSJs.
typedef struct map_extn_list
{
    int num_map_extn;
    struct map_extn *head;
} map_extn_list_t;


/* Nodes that are an extension to the map boundaries. This object will be static in the program.*/
typedef struct map_extn
{

    char name[20];              // name of the extension node
    char *id;                    // id
    time_t start_time;
    int time_of_generation;     // Number of vehicles incoming from the extension.
    struct traffic_sig_jn *adj_signal; // Pointing the immediate traffic signal.
    struct street *connecting_street;
    struct map_extn *next; 
    struct coordinate coord;
      
}map_extn_t;

/*Traffic Signal Light*/
typedef struct traffic_light
{
    char    *id; //unique id of a traffic light
    int     red;//status of red light(on/off)
    int     flag;//storing status
    int     yellow;//status of yellow light(on/off)
    int     forward;//status of green light(on/off) for forward movement of vehicle from a junction
    int     left;//status of green light(on/off) for left  movement of vehicle from a junction
    int     right;////status of green light(on/off) for right movement of vehicle from a junction
    float   traffic_status;//Percentage of the traffic present on the lane.
}traffic_light_t;


/*Street*/
typedef struct street
{
    char    *name;//Name of the street
    char    *id;//Unique ID code for the street
    char    type[10];//Street type description
    int     no[2];//Number of Lanes leading
    struct lane     **lanes[2];/* Lanes leading from lanes[0] : TSJ 1 -to-> TSJ 2 lanes[1] : TSJ 2 -to-> TSJ 1 */
    struct traffic_sig_jn    *tsj[2];/*Pointer to TSJ's the street is joined to. tsj[0] : ->TSJ 1 tsj[1] : ->TSJ 2  */
    struct traffic_light        *tsl[2];/* Pointer of TSL's for the corresponding TSJ's the street is joined to. tsl[0] : ->TSL 1 (2->1) tsl[1] : ->TSL 2  (1->2) */
    struct coordinate   c1;
    struct coordinate   c2;
    struct style        sty;
    

}street_t;

typedef struct lane
{
    char            *id;//Unique ID code for the lane
    struct vehicle  **queue;//Queue of pointers of vehicle
   // int             front;//The position just 1 before the position of the first vehicle
    //int             rear;//The position of the last vehicle
    int             max_vehicles;//Maximum number of vehicles that can be on the lane.
    int             curr_vehicles;//Number of vehicles currently present on the lane
    struct coordinate   c1;
    struct coordinate   c2;
    struct style        sty;
    
 }lane_t;



typedef struct traffic_sig_jn
{

    char        name[20];              // name of the  traffic light
    char        *id;                    // id
    char        type[10];                  // Number of roads connected i.e 1 way, 2 way etc.
    int         flptr;                   //for storing status
    int         mode;
    float       differ;               // stores the difference between current and start
    time_t     start;
    struct street           **streets;         //road from adjacent traffic signal, although no need for it
    struct coordinate       coord;
    struct style            sty;

} traffic_sig_jn_t;





/* Structure to represent each node of the Adjacency List */
typedef struct vertex_node
{
 struct traffic_sig_jn   *dest_jn_p; //Pointer to the destination traffic signal
 struct street              *street_p; //Pointer to the connecting street
 struct adj_list_node       *next; //Pointer to the next adj_list_node of the linked list
}vertex_t;

/* Structure to represent each linked list of the Adjacency List */
typedef struct adj_list
{
 int                        num_of_adj_tsj; /*Number of Adjacent TSJ(Degree of TSJ).*/
 struct vertex_node         *head; //Pointer to the starting(head) node of each linked list
 struct traffic_sig_jn   *src_tsj; //Pointer to the source TSJ node for the edges
 struct adj_list            *next_list; //Pointer to the next adj_list of the graph
}adj_list_t;


typedef struct graph_tsj
{
    int                 num_tsj; /*Number of TSJ’s(vertices).*/
    int                 num_street;  /*Number of Streets(edges).*/
    struct adj_list     tsj_adj_list[MAX_TSJ]; /*Array of pointers of Adjacent list of all vertices to create a linked list of all the vertices a given vertex is connected to. It has size MAX_TSJ.*/
    struct street       *street_list[MAX_STREET]; /*Array of pointers which points to all streets on the graph(map). It has size MAX_STREET.*/
}graph_tsj_t;


typedef struct route
{
    int     no_of_steps; // No. of nodes to cross to take to reach dest.
    struct traffic_sig_jn    **steps; //dynamic array of steps containg by all the junction(traffic signals) by which vehicle pass.
    int     front; // array of first index
    int     rear; // array last index

}route_t;


typedef struct vehicle
{
    //name of the vehicle removed
    char            *id;//unique id for vehicle
    int             speed;//speed of vehicle
    char            type[10];//type of vehicle car,truck or  bike
    struct map_extn *source;//starting map extension node
    struct map_extn *dest;//ending map extension node
    struct route    *directions; // array of all the route to be followed
    char            vehice_no[8];//license plate
    int             priority_status;//priority  vehicles such as vip vehicles
}vehicle_t;

/*------------------------------------------------------------------------------------------------------------------------------------
function definition- in this function traffic light is being created by a street function having argument an id of that street,the id of this traffic light  is obtained by function set_id().the status of all light and directions will be inputted by traffic_light tsl function call by stimulator from the traffic_light pointer ie tsl from the respective street




-------------------------------------------------------------------------------------------------------------------------------------
*/
//traffic_light_t *create_tsl(char *parent_id, int timer); // TIMER TO BE ADDED.


traffic_sig_jn_t *get_tsj_ptr(graph_tsj_t *g,char *tsj_id); 
/* Function:- Generate Number */
char *gen_num(int n)
{
 char *num;                          //a pointer to return the generated number
 num=(char *)malloc(sizeof(char)*5);  //dynamic memory allocation for returning the generated number stored in a character array 
 int i;                                 
 for(i=3;i>=0;i--)                   //loop for storing the number in a character array
 {
  *(num+i)=(char)(n%10 + '0');       //storing the number in character format
  n=n/10;
 }
 *(num+4)='\0';                     
 return num;                         //returning the generated number
}

/*Updated COPY Functions*/
int copy_coordinate(struct coordinate *dest,struct coordinate *src)
{
   
   dest->x         = src->x;
   dest->y         = src->y;
   dest->rotation  = src->rotation;
       
   return 1;
}

int copy_style(struct style *dest,struct style *src)
{
   dest->fill_color    = src->fill_color;
   dest->border_style  = src->border_style;
   dest->border_color  = src->border_color;
   dest->width         = src->width;
       
   return 1;
}


/* Function:- Set ID */
char *set_id(char *parent_id,char *type_id)
{
 static int ctr_men=0;              
 static int ctr_tsj=0;
 static int ctr_lan=0;
 static int ctr_str=0;
 static int ctr_tsl=0;
 static int ctr_veh=0;
 static int ctr_rou=0;
 char *id;
 char *num;
 int id_len=strlen(parent_id)+9;        //storing the length of newly generated id 
 id=(char *)malloc(sizeof(char)*id_len);
 
 strcpy(id,parent_id);                  //copying the parent_id in a pointer named id 
 
 strcat(id,".");                        //adding a dot after id for maintaining the id format

 if(strcmp(type_id,"MEN")==0)          //loop for creating a map extension node type id
 {
     ctr_men++;
     strcat(id,"MEN");
     num=gen_num(ctr_men);
 }
 else if(strcmp(type_id,"TSJ")==0)     //loop for creating a traffic signal junction type id
 {
     ctr_tsj++;
     strcat(id,"TSJ");
     num=gen_num(ctr_tsj);
 }
 else if(strcmp(type_id,"LAN")==0)     //loop for creating a lane type id
 {
     ctr_lan++;
     strcat(id,"LAN");
     num=gen_num(ctr_lan);
 }
 else if(strcmp(type_id,"STR")==0)     //loop for creating a street type id
 {
     ctr_str++;
     strcat(id,"STR");
     num=gen_num(ctr_str);
 }
 else if(strcmp(type_id,"TSL")==0)     //loop for creating a traffic signal light type id
 {
     ctr_tsl++;
     strcat(id,"TSL");
     num=gen_num(ctr_tsl);
 }
 else if(strcmp(type_id,"VEH")==0)     //loop for creating a vehicle type id
 {
     ctr_veh++;
     strcat(id,"VEH");
     num=gen_num(ctr_veh);
 }
 else if(strcmp(type_id,"")==0)   //loop for creating a route type id
 {
     ctr_rou++;
     strcat(id,"ROU");
     num=gen_num(ctr_rou);
 }
 strcat(id,num);
 
 return id;
 
}

vehicle_t **create_queue( int max_vehicles )
{
    int i;
    vehicle_t **ptr;
    ptr=(vehicle_t **)malloc((sizeof(vehicle_t *))*max_vehicles);
    for(i=0;i<max_vehicles;i++){
    	ptr[i] = NULL	 ; 
    }
    return ptr;

}


traffic_light_t *create_tsl(char *parent_id)
{
    traffic_light_t *ptr=NULL;
    
    ptr=(traffic_light_t *)malloc(sizeof(traffic_light_t));
    ptr->id=set_id(parent_id, "TSL");/*id of the street fro it is created being passed into function set_id*/
   /* ptr->timer=0;//int value to be taken by stimulation function/analysis*/
    ptr->red=0;/*all these are initially null and value provided by stimuation function(as it provide the mode) by the pointer*/
    ptr->yellow=0;
    ptr->forward=0;
    ptr->left=0;
    ptr->right=0;
    ptr->flag=0;
    ptr->traffic_status=0;
    //initialize coordinate(ptr,coord) TBD
    return ptr;

}


lane_t *create_lane(char *parent_id,int max_vehicles,int curr_vehicles)
 {
 	struct coordinate *c1;struct coordinate *c2;struct style *sty;
      lane_t *ptr=NULL;
      ptr=(lane_t *)malloc(sizeof(lane_t));
      ptr->id=set_id(parent_id, "LAN");/*id of the street fro it is created being passed into function set_id*/
      ptr->max_vehicles=max_vehicles;
      ptr->curr_vehicles=curr_vehicles;
      ptr->queue=create_queue(max_vehicles);
      copy_coordinate( &(ptr->c1) , c1 );
   copy_coordinate( &(ptr->c2) , c2 );
   copy_style( &(ptr->sty) , sty );

      return ptr;

 }

lane_t **create_n_lane(char *parent_id,int max_vehicles,int curr_vehicles,char *type)
{
       lane_t **ptr;
       if(strcmp(type,"two_way")==0)//whenever type function  would be defined remember to define street type  as "two_way" or "four_way"//
       {
             ptr=(lane_t **)malloc(sizeof(lane_t *)*1);
             ptr[0]=create_lane(parent_id,max_vehicles,curr_vehicles);//forward one way is created

        }
       else if(strcmp(type,"four_way")==0)
       {
            ptr=(lane_t **)malloc(sizeof(lane_t *)*2);
             ptr[0]=create_lane(parent_id,max_vehicles,curr_vehicles);//forward one way is created
             ptr[1]=create_lane(parent_id,max_vehicles,curr_vehicles);

       }
       else if(strcmp(type,"six_way")==0)
       {
             ptr=(lane_t **)malloc(sizeof(lane_t *)*3);
             ptr[0]=create_lane(parent_id,max_vehicles,curr_vehicles);//forward one way is created
             ptr[1]=create_lane(parent_id,max_vehicles,curr_vehicles);
             ptr[2]=create_lane(parent_id,max_vehicles,curr_vehicles);

       }
       return ptr;
}



void queue_add_vehicle( lane_t *l,vehicle_t *v )  // circular queue of array
{
	l->queue[l->max_vehicles-1]=v;

} // add graphic variables.


void *queue_delete(lane_t *l)
{
    l->queue[0]=NULL;
} // add graphics function.




street_t *create_street(char *name, char *type, int max_vehicles, int curr_vehicles, int timer ) // timer to be taken from stimulation function
                                                                                                // add graphics function.
{
    street_t *ptr;
    char *parent_id = NULL;
    ptr = (street_t *)malloc(sizeof(street_t));
    strcpy(ptr->name, name);
    ptr->id = set_id("","STR");
    strcpy(ptr->type, type);
    if(strcmp(type,"two_way")==0)
    {
        ptr->no[0]=1;
        ptr->no[1]=1;
    }

    else if(strcmp(type,"four_way")==0)
    {
        ptr->no[0]=2;
        ptr->no[1]=2;
    }


    else if(strcmp(type,"six_way")==0)
    {
        ptr->no[0]=3;
        ptr->no[1]=3;
    }

    ptr->lanes[0] = create_n_lane(ptr->id ,max_vehicles,curr_vehicles,ptr->type);
    ptr->lanes[1] = create_n_lane(ptr->id ,max_vehicles,curr_vehicles,ptr->type);

    ptr->tsl[0] = create_tsl(ptr->id); // timer to be taken from stimulation.
    ptr->tsl[1] = create_tsl(ptr->id); // timer to be taken from stimulation.

    ptr->tsj[0] = NULL;
    ptr->tsj[1] = NULL;    //If and then take in the argument and add other wise delete it from every where in street.
    return ptr;
}

 


traffic_sig_jn_t *create_tsj(char *name )  // check this adjacency list.
{

    traffic_sig_jn_t *ptr=(traffic_sig_jn_t *)malloc(sizeof(traffic_sig_jn_t)) ;
    char *parent_id = NULL;
    int i;
    struct coordinate *c;struct style *sty;
    ptr->differ=0;
    ptr->start=0;
    ptr->mode=0;
    ptr->flptr=0;
    strcpy(ptr->name,name);
    ptr->id = set_id("", "TSJ");
    strcpy(ptr->type,"FOUR");
    copy_coordinate(&(ptr->coord) , c);
   copy_style(&(ptr->sty) , sty);

   /*  if(strcmp(type,"three_side")==0)
    {
        ptr->no_of_adj_sig = 3;
    }

   else if(strcmp(type,"four_side")==0)
    {
        ptr->no_of_adj_sig = 4;
    }

    else if(strcmp(type,"five_side")==0)
    {
        ptr->no_of_adj_sig = 5;
    }
*/



   /* if(strcmp(type,"three_side")==0)
    {
        i=0;
        ptr->adj_signal = (traffic_sig_jn_t **)malloc((sizeof(traffic_sig_jn_t *))*3);  // to complete this syntax of adjacency list is required.
        while(adjacency_list[id]!= NULL)
            {

            adj_signal[i] = adjacent_l1st[id]->ptr some thing;
            i++;
            }


    }

   else if(strcmp(type,"four_side")==0)
    {
        ptr->adj_signal = (traffic_sig_jn_t **)malloc((sizeof(traffic_sig_jn_t *))*4);
        i=0;
        while(adjacent_l1st[id]->ptr some thing != NULL)
            {

            adj_signal[i] = adjacent_l1st[id]->ptr some thing;
            i++;
            }

    }
*/
   /*else  if(strcmp(type,"five_side")==0)
    {
        ptr->adj_signal = (traffic_sig_jn_t **)malloc((sizeof(traffic_sig_jn_t *))*5);
        i=0;
        while(adjacent_l1st[id]->ptr some thing != NULL)
            {

            adj_signal[i] = adjacent_l1st[id]->ptr some thing;
            i++
            }
    }*/

// will be completed after above one.

   /* struct street **streets_out;           // Pointer to node toward adjacent traffic light,
    struct street **streets_in;         road from adjacent traffic signal, although no need for it*/
    
	ptr->streets=NULL;
	

    return ptr;
}

map_extn_t *create_map_extn(char *name, int time_of_generation , traffic_sig_jn_t *adj_tsj , street_t *connecting_street )
{
    map_extn_t *newnode;
    newnode = (map_extn_t *)malloc(sizeof(map_extn_t));
    strcpy(newnode->name,name);
    newnode->id = set_id("","MEN"); 
    newnode->time_of_generation = time_of_generation;
    newnode->next = NULL;
    newnode->adj_signal = adj_tsj;
    newnode->connecting_street = connecting_street;

    return newnode;
}

map_extn_list_t *create_map_extension_list(void)
{
    
    
    map_extn_list_t *newnode;
    newnode = (map_extn_list_t *)malloc(sizeof(map_extn_list_t));
    newnode->num_map_extn = 0;
    newnode->head = NULL;

    return newnode;
} 


int add_map_extn(char *name, int time_of_generation, traffic_sig_jn_t *adj_tsj, street_t *connecting_street, map_extn_list_t *map_extn_p )
{
    map_extn_t *newnode, *temp,*head;
    newnode = create_map_extn(name,time_of_generation ,adj_tsj ,connecting_street);
    (map_extn_p->num_map_extn)++;
    if (map_extn_p->head == NULL){
        map_extn_p->head = newnode;
    }
    
    else
    {
        temp = head;
        head = newnode;
        newnode->next = temp;
    }
    
    
 return 1;

}


int delete_map_extn(map_extn_list_t *map_extn_p, char *id)
{
    int i=0;
    map_extn_t *temp, *prev;
    temp = map_extn_p->head;
    prev = map_extn_p->head;
    
    while(temp->next != NULL)
    {
        if(strcmp(temp->id,id)==0)
        {
            i++;
            break;
        }
        prev = temp;
        temp = temp->next;
        
    }
    
    
    if(i==0)
    {
        return 0;
    }
    
    else if(map_extn_p->head->id==id)
    {
        
        temp = map_extn_p->head;
        map_extn_p->head = map_extn_p->head->next;
        free(temp);
    }
    
    
    else
    {
        
        prev->next = temp->next;
        free(temp);        
    }

    return 1;

}


map_extn_t *get_map_extn_node(map_extn_list_t *map_extn_p, char *id)
{
 
 int i=0;
    map_extn_t *temp;
    temp = map_extn_p->head;
    
    while(temp->next != NULL)
    {
        if(strcmp(temp->id,id)==0)
        {
            i++;
            break;
        }
        temp = temp->next;
        
    }
    
    
    if(i==0)
    {
        return NULL;
    }
    
    return temp;
 
}


/* Function:- Initialize vehicle */
vehicle_t *create_vehicle()
{
    vehicle_t *ptr;
    ptr=(vehicle_t *)malloc(sizeof(vehicle_t));
    ptr->id=NULL;
    ptr->speed=0;
    ptr->source=NULL;
    ptr->dest=NULL;
    ptr->directions=NULL;
    ptr->priority_status=0;
    
    return ptr;
}
 
 
/*Function to get the index of the TSJ inside the street corresponding to the passesTSJ*/
int get_tsj_index(street_t *s,traffic_sig_jn_t *tsj)
{
    
    int i;
    /*
    Comparing each of the two TSJ's
    */
    for(i=0 ; i<2 ;++i)
    {
        if(s->tsj[i] == tsj)        
        {
            return i;   /*FOUND: TSJ found at index 'i'*/
        }
    }
    
    return -1;  /*FAILURE: TSJ not found*/
}

/*Function to return type of the street as an int*/
int get_street_type(street_t *s)
{
    char two_way[]="two_way";
    char four_way[]="four_way";
    
    if(strcmp(s->type,two_way) == 0)
    {
        return 2;
    }
    else if(strcmp(s->type,four_way) == 0)
    {
        return 4;
    }
    
    /*FAILURE*/
    return -1;
}

float vehicle_type(vehicle_t *v)
{
     if(strcmp(v->type,"Car") == 0 || strcmp(v->type,"Bike") == 0)
    {
        return 1;
    }
    else if(strcmp(v->type,"Bus") == 0)
    {
        return 1.75;
    }
    
    else if(strcmp(v->type,"Truck") == 0)
    {
        return 2.25;
    }
    
    
    /*FAILURE*/
    return -1;
    
}




float set_traffic_status(street_t *s, traffic_sig_jn_t *t)
{
    lane_t ***l;
    int i, j, k, type, sum=0, index ;
    float a, *array;
    int streets;
    index = get_tsj_index(s,t);
    streets = get_street_type(s);
    
    if(index==0)
    {
        *l= s->lanes[index];
    }
    
    else if(index==1)
    {
        *l= s->lanes[index];
    }
    
    if(streets==2)
    {
        k=l[index][0]->curr_vehicles;    
        array =(float *) malloc(sizeof(float)*k);
        for(i=0 ; i<k ; i++)
        {
            array[i] = vehicle_type(l[index][0]->queue[i]);
        }
    
    }
    
    else if(streets==4)
    {
        k= l[index][0]->curr_vehicles + l[index][1]->curr_vehicles;
        array =(float *) malloc(sizeof(float)*k);
        
        type = 0;
        for(j=0;j<2;j++)
        {
            for(i=0 ; i<l[index][j]->curr_vehicles ; i++)
            {
                array[type+i] = vehicle_type(l[index][j]->queue[i]);
            }
            type = i;
        }
    }
    
    for(i=0;i<k;i++)
    {
        sum += array[i];
    }
    
    
    if (streets==2)
    {
        a= (sum*100)/((l[index][0]->max_vehicles)*2.25);    
    }
     
    else if (streets==4)
    {
        a= (sum*200)/((l[index][0]->max_vehicles)*2.25);    

    } 
        
    
    return a;
}


int timer (float array[])
{
    
    int i, j, k, total_time, main_time;
    
    if(array[0]<30.0 && array[0]> 0.00)
    {
        total_time = 120;
    }
    
    else if(array[0]<50.0 && array[0]> 30.0)
    {
        total_time = 180;
    }
    
    else if(array[0]> 50.00)
    {
        total_time = 240;
    }
    
    main_time = (array[0]/(array[0]+array[1]+array[4]+array[3]))*total_time;
    
    return main_time;
}




int is_adjacent(traffic_light_t *travelled[],traffic_light_t *sorted_array[],traffic_light_t *A,traffic_light_t *B,traffic_light_t *C,traffic_light_t *D) // function to check if tls is adjacent or not
{
    if(sorted_array[0]==A && (travelled[0]==B||travelled[0]==D))
    {
        return 1;
    }
    else if(sorted_array[0]==B&&(travelled[0]==A||travelled[0]==C))
    {
        return 1;
    }
    else if(sorted_array[0]==C&&(travelled[0]==B||travelled[0]==D))
    {
        return 1;
    }
    else if(sorted_array[0]==D&&(travelled[0]==A||travelled[0]==C))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void analize_tsj(graph_tsj_t *g,char *tsj_id) // main analize function
{
    time_t current;
    int street_type,i,j,k, index;
    street_t *arr[4];
    float busy[4];
    traffic_sig_jn_t *point;
    traffic_light_t *sorted_array[4], *travelled[3],*temp;
    point=get_tsj_ptr(g,tsj_id);  // pointer of tsj from tsj id
    street_type=get_street_type(arr[0]); // type of street 2 way or 4 way


    /*storing the tls pointer of a tsj in arr*/
    for(i=0;i<4;i++)
    {
        arr[i]=point->streets[i];
    }


/*naming the pointes A, B, C, D to simplify*/
    traffic_light_t *A=arr[0]->tsl[get_tsj_index(arr[0],point)];
    traffic_light_t *B=arr[1]->tsl[get_tsj_index(arr[1],point)];
    traffic_light_t *C=arr[2]->tsl[get_tsj_index(arr[2],point)];
    traffic_light_t *D=arr[3]->tsl[get_tsj_index(arr[3],point)];


/*loop to store traffic status of each tsl by calling function set_traffic status*/
    for(i=0;i<4;i++)
    {
        index = get_tsj_index(arr[i],point);
        arr[i]->tsl[index]->traffic_status = set_traffic_status(arr[i], point);
    }

/*setting A B C D in sorted array and then sorting them acc to their traffic status in below loop.*/
    sorted_array[0]=A;
    sorted_array[1]=B;
    sorted_array[2]=C;
    sorted_array[3]=D;

    for(j=0;j<4;j++)
    {
        for(k=j+1;k<4;k++)
        {
            if((sorted_array[j]->traffic_status)<(sorted_array[k]->traffic_status))
            {
                (sorted_array[j]->traffic_status)+=(sorted_array[k]->traffic_status);
                (sorted_array[k]->traffic_status)=(sorted_array[j]->traffic_status)-(sorted_array[k]->traffic_status);
                (sorted_array[j]->traffic_status)-=(sorted_array[k]->traffic_status);
            }
            else
            {
                k++;
            }
        }
    }

for(i=0;i<4;i++)
{
	busy[i]=sorted_array[i]->traffic_status;
}

/*storing the tsl pointers in travelled array except the most bussiest tsl and then sorting it acc to the flag, whether  visited or not */
    for(i=0;i<3;i++)
    {
        travelled[i]=sorted_array[i+1];
    }


    if(travelled[0]->flag!=0&&travelled[1]->flag!=0&&travelled[2]->flag!=0)
{
    for(i=0;i<3;i++)
        {
            if((travelled[i]->flag==1))
                {
                    temp = travelled[i];
                    travelled[i] = travelled[0];
                    travelled[0] = temp;
                }

        }
}


/*predefining modes of different tsl */

    int mode_type;
     if(sorted_array[0]==A)
    {
        mode_type=14;
    }
    else if(sorted_array[0]==B)
    {
        mode_type=13;
    }
    else if(sorted_array[0]==C)
    {
        mode_type=16;
    }
    else if(sorted_array[0]==D)
    {
        mode_type=15;
    }


/* Stroing current time in current*/
    time(&current);


  /* 1. if of time  */
   if(difftime(current,point->start) >point->differ)
   {
      point->differ=timer(busy);//return difference btw current time and starting time

        /*2. if of street type, in this case 2 way*/
       /* if(street_type==2)// if of type
       {

       }*/

       /*2. if of street type, in this case 4 way*/
       if(street_type==4)// if of type
       {

           // if of mode for base
           if(point->mode!=14&&point->mode!=13&&point->mode!=15&&point->mode!=16)
           {
               /* this stores the timer of current tsl to be executed*/
               point->differ=point->differ*0.75;


               // conditional  ifs  depending upon flag.
               if(travelled[0]->flag==0)
               {
                   set_tsj_mode(A,B,C,D,mode_type);//base condition in 4way.xlsx
                    point->mode=mode_type;
               }

               else
               {
                    if(is_adjacent(travelled,sorted_array,A,B,C,D))//check both are adjacent or not
                    {
                        set_tsj_mode(A,B,C,D,mode_type);//b first_condition in 4way.xlsx
                        point->mode=mode_type;
                    }
                    else
                    {
                        set_tsj_mode(A,B,C,D,mode_type);// c first condition in 4way.xlsx
                        point->mode=mode_type;
                        point->flptr=1;
                    }

               }

                /*Loop to increment the flags of all tsl of a junction after travelling base mode.*/
                for(k=0;k<4;k++)
                {
                    if(sorted_array[k]->flag!=0)
                    {
                        sorted_array[k]->flag=((sorted_array[k]->flag)+1)%4;
                    }
                }
                (sorted_array[0]->flag)++;
           }


            else  // mode for sub mode
            {
                if(point->flptr==1)
                {
                    point->flptr=0;
                    point->mode=0;

                }
                else
                {
                    if(point->mode==14)//A
                    {
                        set_mode(A,B,C,D,18);
                        point->mode=18;
                    }
                    if(point->mode==13)//B
                    {
                        set_mode(A,B,C,D,17);
                        point->mode=17;
                    }
                    if(point->mode==16)//C
                    {
                        set_mode(A,B,C,D,18);
                        point->mode=18;
                    }
                    if(point->mode==15)//D
                    {
                        set_mode(A,B,C,D,17);
                        point->mode=17;
                    }

                }
                point->differ=point->differ*0.25;

            }

       }
       point->start=current;
     }
}


/*  Function:- Set Traffic Signal Junction Mode 
    The function takes pointers to traffic lights at a junction and sets traffic light mode for the respective traffic light */

void set_tsj_mode(traffic_light_t *A,traffic_light_t *B,traffic_light_t *C,traffic_light_t *D,int mode) 
{
    switch(mode)                    /* Cases to pass the modes of traffic lights */
    {
        case 1:
            set_tsl_mode(A,8);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,5);
            break;
        case 2:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;    
        case 3:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,5);
            set_tsl_mode(D,5);
            break;
        case 4:
            set_tsl_mode(A,5);
            set_tsl_mode(B,8);
            set_tsl_mode(C,1);
            set_tsl_mode(D,1);
            break;
        case 5:
            set_tsl_mode(A,1);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,7);
            break;
        case 6:
            set_tsl_mode(A,5);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,5);
            break;
        case 7:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,8);
            set_tsl_mode(D,1);
            break;
        case 8:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;
        case 9:
            set_tsl_mode(A,5);
            set_tsl_mode(B,5);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;
        case 10:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,1);
            set_tsl_mode(D,8);
            break;
        case 11:
            set_tsl_mode(A,1);
            set_tsl_mode(B,7);
            set_tsl_mode(C,1);
            set_tsl_mode(D,7);
            break;
        case 12:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,5);
            set_tsl_mode(D,7);
            break;
        case 13:
            set_tsl_mode(A,4);
            set_tsl_mode(B,8);
            set_tsl_mode(C,4);
            set_tsl_mode(D,4);
            break;
        case 14:
            set_tsl_mode(A,8);
            set_tsl_mode(B,4);
            set_tsl_mode(C,4);
            set_tsl_mode(D,4);
            break;
        case 15:
            set_tsl_mode(A,4);
            set_tsl_mode(B,4);
            set_tsl_mode(C,4);
            set_tsl_mode(D,8);
            break;
        case 16:
            set_tsl_mode(A,4);
            set_tsl_mode(B,4);
            set_tsl_mode(C,8);
            set_tsl_mode(D,4);
            break;
        case 17:
            set_tsl_mode(A,4);
            set_tsl_mode(B,6);
            set_tsl_mode(C,4);
            set_tsl_mode(D,6);
            break;
        case 18:
            set_tsl_mode(A,6);
            set_tsl_mode(B,4);
            set_tsl_mode(C,6);
            set_tsl_mode(D,4);
            break;
        
        
    }
    
}

