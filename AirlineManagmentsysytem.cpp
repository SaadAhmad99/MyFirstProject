#include<iostream>
#include<ctime>
#include<fstream>
using namespace std;

class Ticket{
	private:
		string ticketid;
		int bookingstatus;//0 or 1
		public:
			string getticketid(){
				return ticketid;
			}
		
			int getbookings(){
				return bookingstatus;
			}
		
	Ticket(){
		bookingstatus=0;
	}
	
	void generateticket(){
    srand(time(0));
    string id = "";
    id+='A'+rand()%26;
    id+='A'+rand()% 26;
    id+=to_string(1000 + rand() % 9000); //convert string to 4 digits
    ticketid=id;
    bookingstatus=1;
	cout<<"Ticket is generated"<<endl;	
	}

	void displayTicket(){
		cout<<"\n----Ticket Details:";
		cout<<"----Ticket id= "<<ticketid<<endl;
	}
};
class Pessenger{
	private:
		string name;
		int age;
		string passportnum;
		char gender;
	
	public:
		string getname(){
			return name;
		}
		
		int getage(){
			return age;
		}
		string getpassportnum(){
			return passportnum;
		}
		char getgender(){
			return gender;
		}
	Pessenger(){}

        void inputpassengerinfo(){	 	
	 	cout<<"Enter passenger name:"<<endl;
	 	getline(cin,name);
	 	cout<<"Enter passenger age:"<<endl;
	 	cin>>age;
	 	cout<<"Enter passenger Passport number:"<<endl;
	 	cin.ignore();
	 	getline(cin,passportnum);
	 	cout<<"Enter Gender(M/F):"<<endl;
	 	cin>>gender;
		}
	 
		void displaypassengerinfo(){
	 	cout<<"Passenger name:"<<name;
	 	cout<<"----Passenger age:"<<age;
	 	cout<<"----Passenger Passport number:"<<passportnum;
	 	cout<<"----Gender:"<<gender;
		}
		
};
class Flight{
	protected:
		
		string flightnum;  
		string airlinename;
		string source;
		string destination;
		int availableseat;
		double ticketprice;
		string ftype;
		
		public:
			//geeters
			
			string getflightnum(){
				return flightnum;
			}
			string getairlinename(){
				return airlinename;
			}	
			string getsource(){
				return source;
			}
			string getdestination(){
				return destination;
			}	
			int getavailableseat(){
				return availableseat;
			}	
			double getticketprice(){
				return ticketprice;
			}	
			string getftype(){
				return ftype;
			}
		int pessengercount;
		int ticketcount;
		Pessenger* p=nullptr;
		Ticket* t=nullptr;
		
		//operator ooverloading <<
		friend ostream& operator<<(ostream& out,Flight &f){
			out<<" \n Flight Type:" <<f.getftype()
             << "\nFlight No: " << f.getflightnum() << " | "
             << "Airline: " << f.getairlinename() << "\n"
             << "From: " << f.getsource() << " -> "
             << "To: " << f.getdestination() << "\n"
             << "Seats: " << f.getavailableseat() << " | "
             << "Price: $" << f.getticketprice() << "\n";
             return out;
		}
	
		Flight(){	
		pessengercount=0;
		ticketcount=0;
		}
		
	Flight(string fnum, string aname, string source, string destination, int seats, double price){
		flightnum=fnum;
		airlinename=aname;
		this->source=source;
		this->destination=destination;
		availableseat=seats;
		ticketprice=price;
		pessengercount=0;
		ticketcount=0;
		ftype="Domestic";
	}
     
    void bookflight(){
    	Pessenger* pnew=new Pessenger[pessengercount+1];
    	
    	for(int i=0;i<pessengercount;i++){
    		pnew[i]=p[i];
		}
		delete[] p;
		p=pnew;
		cout<<"\nBooking for Passenger #"<<pessengercount+1<<endl;
		pnew[pessengercount].inputpassengerinfo();
		pessengercount++;
		cout<<"Booking successful!\n"<<endl;
		
		Ticket* tnew=new Ticket[ticketcount+1];
		for(int i=0;i<ticketcount;i++){
			tnew[i]=t[i];
		}
		delete[] t;
		tnew[ticketcount].generateticket();
		t=tnew;
		ticketcount++;
		displayflightdetails();
		
    }
	
	void cancelflight(){
	string num;
	cout<<"Enter Passport number to cancel flight:";
	getline(cin,num);
	int index=-1;
	bool check=false;
	
	for(int i=0;i<pessengercount;i++){
		if(p[i].getpassportnum()==num){
			check=true;
			index=i;
			break;
		}
	}	
		if(check){
			Pessenger* pn=new Pessenger[pessengercount-1];
			int j=0;
			Ticket *tn=new Ticket[ticketcount-1];
			for(int i=0;i<ticketcount;i++){
				if(index==i)continue;
				tn[j]=t[i];
				pn[j++]=p[i];
			}
			delete[] p;
			p=pn;
			delete[] t;
			t=tn;
			ticketcount--;
			pessengercount--;
			cout<<"Ticket cancelled sucessfully!"<<endl;
		}
		else{
			cout<<"Pessenger not found"<<endl;
		}
	}
		
	void displayflightdetails(){
		p[pessengercount-1].displaypassengerinfo();
		t[ticketcount-1].displayTicket();
	}

	void displaypessenger(){
		for(int i=0;i<pessengercount;i++){
			cout<<"Passenger name:"<<p[i].getname();
	 		cout<<"----Passenger age:"<<p[i].getage();
	 		cout<<"----Passenger Passport number:"<<p[i].getpassportnum();
	 		cout<<"----Gender:"<<p[i].getgender();
	 		cout<<"----Ticket id= "<<t[i].getticketid()<<endl;
	 	
		}
	}

    friend istream& operator>>(istream& in,Flight& f){
    

     cout<<"Enter flight number:\n";
     getline(in,f.flightnum);
    cout<<"Enter Airline Name: ";
    getline(in, f.airlinename);

    cout<<"Enter Source: ";
    getline(in, f.source);

    cout<<"Enter Destination: ";
    getline(in, f.destination);

    cout<<"Enter Number of Seats: ";
    in>>f.availableseat;

    cout<<"Enter Ticket Price: ";
    in>>f.ticketprice;
    in.ignore();
    cout<<"Enter Flight type(International/Domestic):";
    getline(in,f.ftype);
    
    return in;
    
	}
	
	Flight(const Flight& other) {
    	flightnum = other.flightnum;
    	airlinename = other.airlinename;
    	source = other.source;
    	destination = other.destination;
    	availableseat = other.availableseat;
    	ticketprice = other.ticketprice;
    	ftype = other.ftype;
    	pessengercount = other.pessengercount;
    	ticketcount = other.ticketcount;

    	p = new Pessenger[pessengercount];
    	for (int i = 0; i < pessengercount; i++)
        	p[i] = other.p[i];

    	t = new Ticket[ticketcount];
    	for (int i = 0; i < ticketcount; i++)
        	t[i] = other.t[i];
	}

	~Flight(){

		delete[] t;
		delete[] p;
	
		t=nullptr;
		p=nullptr;
	}
};

class Domesticflight:public Flight {  
  public:
  		Domesticflight(string fnum, string aname, string source, string destination, int seats, double price):Flight(fnum,aname,source,destination,seats,price){
  		  ftype="Domestic";
		  }
		  Domesticflight(){}
		  
          Domesticflight& operator=( Flight& other){
			if(this!=&other){ 
	
	  flightnum = other.getflightnum();
        airlinename = other.getairlinename();
        source = other.getsource();
        destination = other.getdestination();
        availableseat = other.getavailableseat();
        ticketprice = other.getticketprice();
        ftype = other.getftype();
		}
    	
    return *this;
	}
  	
};  

class Internationalflight:public Flight {  
public:
   string visa; 
   Internationalflight(){}
  		Internationalflight(string fnum, string aname, string source, string destination, int seats, double price,string visa):Flight(fnum,aname,source,destination,seats,price){
  			this->visa=visa;
  			ftype="International";
		}
		
	Internationalflight& operator=(Flight& other){
		if(this!=&other){
		 
	  flightnum = other.getflightnum();
        airlinename = other.getairlinename();
        source = other.getsource();
        destination = other.getdestination();
        availableseat = other.getavailableseat();
        ticketprice = other.getticketprice();
        ftype = other.getftype();
    }
    return *this;
	}
};
class Airport{
	public:
	 string airportname;
	 int size=0;
	
	 Flight* f=nullptr;
	 
	 //add flight 
	 void addflight(Flight& ff){
	 	
	 Flight* fnew= new Flight[size+1];
	 
     for(int i=0;i<size;i++){
    	fnew[i]=f[i];
	 }
	  fnew[size]=ff;
	  delete[] f;
	  f=fnew;
      size++;
	}
	
	void inputandadd(){	
		Flight f;
		cin.ignore();
		cin>>f;
		if(f.getftype()=="International"||f.getftype()=="international"){
        
        Internationalflight newFlight;
        newFlight=f;
        addflight(newFlight);
        
    }else{
        Domesticflight newFlight;
        newFlight=f;
        addflight(newFlight);
		
	}}
   
	void displayallflights(){
		if(size==0){
        cout << "No flights available.\n";
        return;
    	}
    	for(int i=0;i<size;i++){
        	cout <<"\n=== Flight " << i + 1 << " ===\n";
             cout<<f[i];
    	}
    }
	
	void searchflight(){
		if(size==0){
        cout << "No flights available.\n";
        return;
    	}
		
		cout<<"Search Flight By:"<<endl;
		int choice;
    	cout<<"\n1. Flight Number\n2. Source & Destination\n3. Airline Name "<<endl;
    	cin >> choice;
    
      	bool s=false;
      	int index;
      	bool again=false;

    switch(choice){

        case 1:
		{
         string num;
         cout<<"Enter Flight number:";
         cin.ignore();
         getline(cin,num);
    	for(int i=0;i<size;i++){
            if(f[i].getflightnum()==num){
            	index=i;
            	s=true;
			
                    
            }
			if(s){ 
		
		 	 cout<<"\n=== Flight " << i + 1 << " ===\n"
		 	 <<" \n Flight Type:"<<f[i].getftype()
             <<"\nFlight No: "<<f[index].getflightnum()<< " | "
             <<"Airline: "<<f[index].getairlinename()<< "\n"
             <<"From: "<<f[index].getsource()<< " -> "
             <<"To: "<<f[index].getdestination()<< "\n"
             <<"Seats: "<<f[index].getavailableseat()<< " | "
             <<"Price: $"<<f[index].getticketprice()<< "\n";
			 s=false;
			 again=true;
			}
		}
			if(!again){
				cout<<"Flight not Found."<<endl;		
			}	
		}
        	break;

        case 2:
        {
        	string sou,d;
        	cout<<"Enter source:";
        	cin.ignore();
        	getline(cin,sou);
        	cout<<"Enter Destination:";
        	getline(cin,d);
        	for(int i=0;i<size;i++){
            	if(f[i].getsource()==sou&&f[i].getdestination()==d){
            		index=i;
            		s=true;
               	}
				
        	if(s){
			
		 	 cout<<"\n=== Flight " << i + 1 << " ===\n"
		 	 <<" \n Flight Type:"<<f[i].getftype()
             <<"\nFlight No: "<<f[index].getflightnum()<< " | "
             <<"Airline: "<<f[index].getairlinename()<< "\n"
             <<"From: "<<f[index].getsource()<< " -> "
             <<"To: "<<f[index].getdestination()<< "\n"
             <<"Seats: "<<f[index].getavailableseat()<< " | "
             <<"Price: $"<<f[index].getticketprice()<< "\n";
             s=false;
             again=true;
			}
		}
				 if(!again){
				 
					cout<<"Flight not Found."<<endl;		
	}	
		}
            break;

        case 3:
        {	
        	string a;
        	cout<<"Enter Airline Name:";
        	cin.ignore();
        	getline(cin,a);
        	for(int i=0;i<size;i++){
            if(f[i].getairlinename()==a){
            		index=i;
            		s=true;
            }
				
        	if(s){
       	 cout<<"\n=== Flight " << i + 1 << " ===\n"
		 	 <<" \n Flight Type:"<<f[i].getftype()
             <<"\nFlight No: "<<f[index].getflightnum()<< " | "
             <<"Airline: "<<f[index].getairlinename()<< "\n"
             <<"From: "<<f[index].getsource()<< " -> "
             <<"To: "<<f[index].getdestination()<< "\n"
             <<"Seats: "<<f[index].getavailableseat()<< " | "
             <<"Price: $"<<f[index].getticketprice()<< "\n";
             s=false;
             again=true;
			}
			}
			if(!again){ 
				cout<<"Flight not Found."<<endl;		
			}	
		}       	
        break;

        default:
            cout << "Invalid choice." << endl;
    }
    cout<<endl;
}
 void deleteflight(){
 	 if (size==0) {
        cout << "No flights available.\n";
        return;
    }
	Flight* fneww=new Flight[size-1];
	string fname;
	cin.ignore();
	cout<<"Enter flight number to delete Flight:"<<endl;
	getline(cin,fname);

	bool c=false;
	int index;
	for(int i=0;i<size;i++){
		if(f[i].getflightnum()==fname){
			index=i;
			c=true;
			
		}
	
	}
	if(c){
		int j=0;
		for(int i=0;i<size;i++){
			if(i==index)continue;
			fneww[j++]=f[i];
			
		}
		delete[] f;
		f=fneww;
		size--;
		cout<<"Flight canceled sucessfully!!!\n";
	}
	else{
		cout<<"Flight not found\n"<<endl;
	}
	
}
void saveToFile() {
    ofstream file("Data.txt");
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        file<<"\n=== Flight " << i + 1 << " ===\n"
		 	 <<" \n Flight Type:"<<f[i].getftype()
             <<"\nFlight No: "<<f[i].getflightnum()<< " | "
             <<"Airline: "<<f[i].getairlinename()<< "\n"
             <<"From: "<<f[i].getsource()<< " -> "
             <<"To: "<<f[i].getdestination()<< "\n"
             <<"Seats: "<<f[i].getavailableseat()<< " | "
             <<"Price: $"<<f[i].getticketprice()<< "\n";
             
        for (int j = 0; j < f[i].pessengercount; j++) {
            file << "\n  -- Passenger " << j + 1 << " --\n"
                 << "  Name: " << f[i].p[j].getname() << "\n"
                 << "  Age: " << f[i].p[j].getage() << "\n"
                 << "  Passport: " << f[i].p[j].getpassportnum() << "\n"
                 << "  Gender: " << f[i].p[j].getgender()<< "\n"
                 << "  Ticket ID: " << f[i].t[j].getticketid() << "\n";
        }
    }

    file.close();
}
~Airport(){
	
		delete[] f;
		f=nullptr;
}

	
};


int main(){
	
	ofstream file("Data.txt");
	Airport a;
	a.airportname="Islamabad";
	
	Domesticflight f1("Pk301","PIA","Haripur","Quetta",30,10000);
	a.addflight(f1);
	
	Domesticflight f2("Pk302","Saudi Arabia","Swat","Islamabad",20,20000);
	a.addflight(f2);
	
	Internationalflight f3("Pk303","Emirates","Karachi","Dubai",50,30000,"yes");
	a.addflight(f3);
	
	bool s=false;
	 
	a.saveToFile();
	int choice;
	cout << "\n==========================================================\n";
	cout << "     Airline Reservation System for Airport: " << a.airportname << endl;
	cout << "==========================================================\n\n";

do {
    cout<<"======================== Main Menu =======================\n";
    cout<<"  1. View All Flights\n";
    cout<<"  2. Search Flight\n";
    cout<<"  3. Book Flight\n";
    cout<<"  4. Cancel Ticket\n";
    cout<<"  5. View All Passengers\n";
    cout<<"  6. Add Flight\n";
    cout<<"  7. Delete Flight\n";
    cout<<"  8. Update Flight\n";
    cout<<"  9. Exit\n";
    cout<<"=============================================\n";
    cout<<"Enter your choice (1-9): ";
    cin>>choice;
    cout<<endl;


    switch(choice){
        case 1:
            a.displayallflights();
            break;

        case 2:
        	a.searchflight();
            break;

        case 3:
        {
        	if(a.size==0){
        		cout<<"No flight available\n";
        		break;
			}
        	 else{
        	 	bool s=false;
        a.displayallflights();
            string choice;
            cout<<"Enter Flight number to Book:"<<endl;
            cin.ignore();
            getline(cin,choice);
            int index;
          
            for(int i=0;i<a.size;i++){
            	if(a.f[i].getflightnum()==choice){
            		index=i;
            		s=true;
            		break;
               
				}
			}
				if(s){
					if(a.f[index].getftype()=="International"||a.f[index].getftype()=="international"){
						string checkvisa;
						 cout<<"This is an International Flight. Visa is required.\n";
                         cout<<"Do you have a valid visa? (yes/no): ";
                         getline(cin,checkvisa);
                         

                    if(checkvisa=="no"){
                     cout<<"Booking denied. Visa is required for international travel.\n";
                    
					}
					else{
				 	a.f[index].bookflight();
				 	a.saveToFile();	
			}
			     }else {
			     	 a.f[index].bookflight();
			     	 a.saveToFile();	
			     	
				 } 
				}
				 else
					cout<<"Flight number not Found."<<endl;	
				}
	}
	break;

        case 4:
        	{ 
        		if(a.size==0){
        		cout<<"No flight available\n";
        		break;
			}
			else{
			 string choices;
        	bool check=false;
            cout<<"Enter Flight number to cancel ticket:"<<endl;
            cin.ignore();
            getline(cin,choices);
            int indexs;
          
            for(int i=0;i<a.size;i++){
            	if(a.f[i].getflightnum()==choices){
            		indexs=i;
            		check=true;
            		break;
               
				}
				}
				if(check){
        		  a.f[indexs].cancelflight();	
				  a.saveToFile();	
			}
			else 
			cout<<"Flight not found"<<endl;
		}
		}
            break;
            
         case 5:
           {  
		    bool check=false;
           	  for(int i=0;i<a.size;i++){
           	  	if(a.f[i].pessengercount!=0){
                	cout<<"Displaying Pessengers for Flight "<<i+1<<endl;
                 	a.f[i].displaypessenger();
                 	check=true;
					 }
                   }
                   if(!check)
				   cout<<"No Pessenger available"<<endl;
		   }
            break;
            
        case 6:
        	{
        	a.inputandadd();
        	a.saveToFile();
        	cout << "Flight added successfully!\n";
    }
	break;

     case 7:
	          {
	          	a.deleteflight();
	          	a.saveToFile();	
			  }
            
            break;
    case 8:
           {
           	string number;
           	int index;
           	bool check=false;
           	cin.ignore();
           	cout<<"Enter flight number to update data:"<<endl;
           	getline(cin,number);
           	for(int i=0;i<a.size;i++){
           		if(a.f[i].getflightnum()==number ){
           			index=i;
           			check=true;
				   }
			   }
			   if(check){
			   	cout<<"Enter updated data:"<<endl;
			   	cin>>a.f[index];
			   	cout<<"Flight updated sucessfully\n";
			   	a.saveToFile();
			   }
			   else{
			   	cout<<"Flight not avaiable\n";
			   }
}
            
            break;
            
            case 9:{
            	
            cout << "Exiting..." << endl;
			
			}
				break;
                        
        default:
            cout << "Invalid choice." << endl;
    }

  }  while(choice != 9);
  
    a.saveToFile();
	
	return 0;
	
}