//  Freshworks_dB 
//  Ahamed Abdullah

#include <iostream>
#include <ctime>

using namespace std;

class JSON{

protected:

    string data;
    JSON(string x) :  data(x) { }
    JSON ( ) { }

};

class Data : private JSON{

private:

    string key;
    time_t time_to_live;

    friend string readkey(Data* obj){ return obj->key; }
    friend time_t readtime_to_live(Data* obj){ return obj->time_to_live; }    
    
public:

    Data (string input_key, string input_value, time_t input_time_to_live) : key(input_key), JSON(input_value), time_to_live(input_time_to_live){ }
    
    Data () { }
    
    string getdata(){ return JSON::data; }
   
    void updatevalue(string input_value){

        JSON::data=input_value;

    }

};

struct Node{

    Node* link;
    Data* addr;

};

class ll{

private:

    Node* head=NULL;

public:

    friend Node* gethead (ll* obj){
        // used to get head, needed for getdata in dB
        return obj->head;
    }

    friend void removehead(ll* obj){
        obj->head= NULL;
    }

    void putdata(Data* input){

        Node* temp= new Node();

        temp->addr = input;
        temp->link = NULL;

        if(head==NULL){
            head = temp;
            return;
        }

        Node* t= head;

        while (t->link != NULL)
            t=t->link;
        
        t->link = temp;

    }

};

class dB {

private:

    ll* arr[1069]{NULL};

    unsigned int total_data= 13888889;

    short int hashing_function(string key){

        // This function returns the index where we need to store the data
        // Takes the key as a input, converts the key into sum of its ascii values and divides it by a prime number to give the index 

        short int index=0;

        for (int i=0; i<key.length();i++)
            index += (int)key[i];

        return index%1069;
    }

public:

    bool putdata(string key,string value,time_t time_to_live = 0){

        //checking if the size limit 1GB is reached
        if(total_data < 0){
            return false;
        }

        Data* temp_Data=new Data;
        
        //checking if the key already exisits in dB
        temp_Data = getdata(key);

        if(temp_Data == NULL){
            
            short int index = hashing_function(key);

            temp_Data=new Data(key, value, time_to_live);
            
            //decrement 1 from total number of data objects it can have
            total_data--;

            //populates the arr if the index is empty 
            if(arr[index]==NULL){
                ll* temp=new ll;
                arr[index] = temp;
            }

            //adds the address of the data to the linked list
            arr[index]->putdata(temp_Data);

            return true;
        }

        else {

            cout<<"Given key already exists in dB"<<endl;
            return false;
        }
    }

    Data* getdata(string key){

        short int index = hashing_function(key);


        //checks if the arr is empty means theres is no data for the given key
        if(arr[index]==NULL)
            return NULL;

        else{

            Node* t= gethead(arr[index]);

            //traverse till the last node in the linked list
            while(t->link != NULL){

                if(key == readkey(t->addr)){
        
                    time_t timenow,time_to_live;
                    timenow=time(0);
                    time_to_live= readtime_to_live(t->addr);
                    
                    //if found the key check its time to live 
                    if (timenow > time_to_live && time_to_live != 0){

                        removedata(key);
                        return NULL;

                    }

                    return t->addr;
                }
            }

            // checks if the last node has the given key
            if(key == readkey(t->addr)){

                time_t timenow,time_to_live;
                timenow=time(0);
                time_to_live= readtime_to_live(t->addr);

                //checks the time to live 
                if (timenow > time_to_live && time_to_live != 0){

                    removedata(key);
                    return NULL;

                }

                return t->addr;
            }
            
        }

        return NULL;

    }

    bool updatedata(string key, string value){

        Data* temp= new Data;

        temp = getdata(key);

        //check if the key is present in dB if not returns false
        if(temp==NULL)
            return false;

        //else updates the value for the given key
        temp->updatevalue(value);

        return true;
    
    }


    bool removedata(string key){ 

        short int index= hashing_function(key);

        //checks if the arr is empty means theres is no data for the given key
        if (arr[index] == NULL) 
            return false;

        Node* t=gethead(arr[index]);
        Node* previousNode = NULL;

        // this if condition for only one node in the linked list
        if(t->link == NULL){

            removehead (arr[index]);
            total_data++;
            delete t->addr;
            delete t;
            arr[index]= NULL;
            return true;

        }

        //this will traverse till the last node
        while(t-> link != NULL){

            //check if the current node has the key
            if(key == readkey(t->addr)){
                total_data++;
                //deallocates memory of Data
                delete t->addr;
                //deallocates the memory of linked list
                delete t;
                //removes the link in the linked list
                previousNode->link=t->link;
                return true;
            }

            previousNode=t;
            t=t->link;
        }
        
        //checks if the last node has the key 
        if(key == readkey(t->addr)){

            total_data++;
            //deallocates memory of Data
            delete t->addr;
            //deallocates the memory of linked list
            delete t;
            //removes the link in the linked list
            previousNode -> link = NULL;
            return true;

        }

        return false;
    } 
    
};



int main(){
    dB MYDB; 
    short int ch;
    string key,value;
    time_t timenow,time_to_live;
    unsigned short int t;
    
    Data* temp = new Data;


    while (true){

        cout<<"Choose Operation \n 1.Enter Data \n 2.Read Data \n 3.Update Data \n 4.Delete Data \n -1.Exit"<<endl;
        cin>>ch;

        switch (ch){

        case 1:
            
            cout<<"Enter Key:";
            cin>>key;
            cout<<"Enter value:";
            cin>>value;
            cout<<"Enter time in mins:";
            cin>>t;
            
            if(t>0){

                timenow=time(0);
                time_to_live= timenow + t*60;
                
                if(MYDB.putdata(key,value,time_to_live))
                    cout<<"Ops Success"<<endl;
                    
                else
                   cout<<"Something bad happend"<<endl;     

            }

            else {

                if(MYDB.putdata(key,value))
                    cout<<"Ops Success"<<endl;
                else
                    cout<<"Something bad happend"<<endl;

            }

            
            break;

        case 2:

            cout<<"Enter key:";
            cin>>key;
            temp = MYDB.getdata(key);
            
            if(temp==NULL)
                cout<<"Given key is either invalid or expaired"<<endl;

            else
                cout<<temp->getdata()<<endl;

            break;

        case 3:

            cout<<"Enter Key:";
            cin>>key;
            cout<<"Enter Value:";
            cin>>value;

            if(MYDB.updatedata(key,value))
                cout<<"Ops Success"<<endl;

            else
                cout<<"Something bad happend"<<endl;

            break;

        case 4:

            cout<<"Enter Key:";
            cin>>key;

            if(MYDB.removedata(key))
                cout<<"Ops success"<<endl;

            else
                cout<<"Given key is either invalid or expaired"<<endl;

            break;

        case -1:

            return 0;
            
        default:

            cout<<"Bad Option"<<endl;
            break;

        }

    }

}