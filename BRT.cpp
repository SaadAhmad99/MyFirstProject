#include <iostream>
#include <string>
using namespace std;

class Time{
public:
    int hour,min,sec;

    Time(){ 
        hour=min=sec=0; 
    }
    Time(int h, int m, int s)
    {
        hour=h;
        min=m;
        sec=s;
    }

    int toSeconds() const{
        return hour*3600 + min*60 + sec;
    }

    static Time fromSeconds(int total){
        if(total<0){
            return Time(0, 0, 0);
        }
        int h=total / 3600;
        total %= 3600;
        int m=total / 60;
        int s=total % 60;
        return Time(h,m,s);
    }

    Time addSecs(int s) const{
        return fromSeconds(toSeconds() + s);
    }

    bool isAfter(const Time &other) const{
        return toSeconds() > other.toSeconds();
    }

    void display() const{
        cout<<(hour<10 ? "0" : "")<<hour<<":"
            <<(min<10 ? "0" : "")<<min<<":"
            <<(sec<10 ? "0" : "")<<sec;
    }
};

class Passenger{
public:
    int id;
    string name;
    Passenger* next;

    Passenger(int pID=0, string pname=""){
        id=pID;
        name=pname;
        next=NULL;
    }
};

class Station{
public:
    int id;
    string name;
    bool isER;
    int srTime;
    int erTime;
    int erTravelToNext;
    int travelTimeToNextER;

    Station *nextSR;
    Station *prevSR;
    Station *nextER;
    Station *prevER;

    Passenger* waitingHead;

    Station(int i=0, string n=""){
        id=i;
        name=n;
        isER=false;
        srTime=erTime=0;
        erTravelToNext=travelTimeToNextER=0;
        nextSR=prevSR=nextER=prevER=NULL;
        waitingHead=NULL;
    }
};

class Bus{
public:
    string type;
    string busID;
    int capacity;
    Station *currentStation;
    Passenger* onboardHead;
    Bus *next;
    Bus *prev;

    Bus(string t="", string id="", int cap=0){
        type=t;
        busID=id;
        capacity=cap;
        currentStation=NULL;
        onboardHead=NULL;
        next=prev=NULL;
    }
};

class BRTManagementsystem{
private:
    Station *srHead;
    Station *erHead;
    Time startTime;
    Time endTime;

public:
    BRTManagementsystem(){
        srHead=erHead=NULL;
        startTime=Time(6, 0, 0);
        endTime=Time(22, 0, 0);
    }

    // ---------- ADD STATION ----------
    void addStation(int id, string name){
        Station *newStation=new Station(id, name);

        if(srHead == NULL){
            srHead=newStation;
        }else{
            Station *temp=srHead;
            while(temp->nextSR != NULL){
                temp=temp->nextSR;
            }
            temp->nextSR=newStation;
            newStation->prevSR=temp;
        }

        string ERlist[]={
                        "Chamkani", "Sardar Garhi", "Lahore Adda", "Hashtnagri","Malik Saad Shaheed", "Khyber Bazar", 
                        "Dabgari Gardens","Saddar Bazar", "University of Peshawar","Mall of Hayatabad", "Karkhano Market"
                        };

        for(int i=0; i<11; i++){
            if(name == ERlist[i]){
                newStation->isER=true;
                if(erHead == NULL){
                    erHead=newStation;
                }else{
                    Station *e=erHead;
                    while(e->nextER != NULL){
                        e=e->nextER;
                    }
                    e->nextER=newStation;
                    newStation->prevER=e;
                }
                break;
            }
        }
        cout<<"Station added: "<<name<<endl;
    }

    void removeStation(int id){
        Station *temp=srHead;
        while(temp && temp->id != id)
            temp=temp->nextSR;

        if(!temp){
            cout<<"Station not found!"<<endl;
            return;
        }

        if(temp->prevSR){
            temp->prevSR->nextSR=temp->nextSR;
        }else{
            srHead=temp->nextSR;
        }
        if(temp->nextSR){
            temp->nextSR->prevSR=temp->prevSR;
        }
        cout<<"Station removed: "<<temp->name<<endl;
        delete temp;
    }

    void displayFullMap(){
        if(!srHead){
            cout<<"No stations found!"<<endl;
            return;
        }
        Station *temp=srHead;
        cout<<"\n=========== FULL MAP(SR-02) ==========="<<endl;
        while(temp){
            cout<<"Station ID: "<<temp->id<<" | Name: "<<temp->name<<endl;
            cout<<"ER Supported: "<<(temp->isER ? "YES" : "NO")<<endl;
            
            Passenger* p=temp->waitingHead;
            int count=0;
            while(p){
                count++;
                p=p->next;
            }
            cout<<"Waiting Passengers: "<<count<<endl;
            cout<<"-----------------------------------"<<endl;
            temp=temp->nextSR;
        }
    }

    void displayExpressMap(){
        if(!erHead){
            cout<<"No ER stations found!"<<endl;
            return;
        }
        Station *temp=erHead;
        cout<<"\n=========== EXPRESS MAP(ER-01) ==========="<<endl;
        while(temp){
            cout<<temp->name;
            if(temp->nextER){
                cout<<" -> ";
            }
            temp=temp->nextER;
        }
        cout<<""<<endl;
    }

    float calculateFare(string from, string to, string busType){
        Station *temp=(busType == "SR") ? srHead : erHead;
        Station *start=nullptr;
        Station *end=nullptr;

        while(temp){
            if(temp->name == from){
                start=temp;
            }if(temp->name == to){
                end=temp;
            }
            temp=(busType == "SR") ? temp->nextSR : temp->nextER;
        }

        if(!start || !end){
            cout<<"Invalid station name(s)!"<<endl;
            return 0;
        }
        if(from == to){
            cout<<"You're already at "<<from<<"! Fare=Rs 0"<<endl;
            return 0;
        }

        int distance=0;
        bool forwardFound=false;
        temp=start;
        while(temp && temp != end){
            distance++;
            temp=(busType == "SR") ? temp->nextSR : temp->nextER;
            forwardFound=true;
        }

        // If not found in forward, check backward
        if(!temp){
            distance=0;
            temp=start;
            while(temp && temp != end){
                distance++;
                temp=(busType == "SR") ? temp->prevSR : temp->prevER;
            }
        }

        if(!temp){
            cout<<"No direct connection between "<<from<<" and "<<to<<"!"<<endl;
            return 0;
        }

        float farePerStation=(busType == "SR") ? 3.0 : 2.5;
        float fare=distance*farePerStation;

        cout<<"Fare from "<<from<<" to "<<to
            <<"("<<busType<<"): Rs "<<fare<<endl;

        return fare;
    }

    void calculateTravelTime(string from, string to, string busType){
        Station *head=(busType == "SR") ? srHead : erHead;
        if(!head){
            cout<<"No "<<busType<<" route found!"<<endl;
            return;
        }

        Station *start=nullptr;
        Station *end=nullptr;
        Station *temp=head;

        while(temp){
            if(temp->name == from){
                start=temp;
            }if(temp->name == to){
                end=temp;
            }
            temp=(busType == "SR") ? temp->nextSR : temp->nextER;
        }

        if(!start || !end){
            cout<<"Invalid station name(s)!"<<endl;
            return;
        }
        if(from == to){
            cout<<"You're already at "<<from<<"!"<<endl;
            return;
        }

        int travelPerStation=(busType == "SR") ? 137 : 120;
        int totalTime=0;
        bool forwardFound=false;

        // Try forward direction
        temp=start;
        while(temp && temp != end){
            totalTime += travelPerStation;
            temp=(busType == "SR") ? temp->nextSR : temp->nextER;
            forwardFound=true;
        }

        // If not found forward, try backward
        if(!temp){
            totalTime=0;
            temp=start;
            while(temp && temp != end){
                totalTime += travelPerStation;
                temp=(busType == "SR") ? temp->prevSR : temp->prevER;
            }
        }

        if(!temp){
            cout<<"No direct connection between "<<from<<" and "<<to<<"!"<<endl;
            return;
        }

        Time arrival=startTime.addSecs(totalTime);

        cout<<"\nTravel time from "<<from<<" to "<<to
            <<"("<<busType<<"): "
            <<totalTime / 60<<" minutes(";
        arrival.display();
        cout<<")"<<endl;
    }

    void findBestRoute(string from, string to){
        int srSegTime=137;
        int transfer=300;

        Station *startSR=NULL, *endSR=NULL, *temp=srHead;
        while(temp){
            if(temp->name == from){
                startSR=temp;
            }if(temp->name == to){
                endSR=temp;
            }
            temp=temp->nextSR;
        }
        if(!startSR || !endSR){
            cout<<"Invalid stations!"<<endl;
            return;
        }

        int srTime=0;
        Station *curr=startSR;
        while(curr && curr != endSR){
            srTime += srSegTime;
            curr=curr->nextSR;
        }

        int erTime=srTime + transfer;
        Time srArrival=startTime.addSecs(srTime);
        Time erArrival=startTime.addSecs(erTime);

        cout<<"\n--- ROUTE COMPARISON ---"<<endl;
        cout<<"SR-02 travel: "<<srTime<<" sec(";
        srArrival.display();
        cout<<")\nER-01 travel: "<<erTime<<" sec(";
        erArrival.display();
        cout<<")\nRecommended: "<<((srTime <= erTime) ? "SR-02" : "ER-01")<<""<<endl;
    }

    struct User{
        string name, currentStation, busType, direction;
        int startTime;
    };

    void reverseRoute(string busType){
        Station *head=(busType == "SR") ? srHead : erHead;
        if(!head){
            cout<<"No "<<busType<<" route found."<<endl;
            return;
        }

        Station *curr=head;
        Station *prev=NULL;
        Station *next=NULL;

        while(curr){
            if(busType == "SR"){
                next=curr->nextSR;
                curr->nextSR=curr->prevSR;
                curr->prevSR=next;
            }else{
                next=curr->nextER;
                curr->nextER=curr->prevER;
                curr->prevER=next;
            }
            prev=curr;
            curr=next;
        }

        if(busType == "SR"){
            srHead=prev;
        }else{
            erHead=prev;
        }
        cout<<busType<<" route reversed successfully!"<<endl;
    }

    void showNextStation(string current, string busType, string direction){
        Station *curr=(busType == "SR") ? srHead : erHead;
        while(curr && curr->name != current){
            curr=(busType == "SR") ? curr->nextSR : curr->nextER;
        }

        if(!curr){
            cout<<"Station "<<current<<" not found!"<<endl;
            return;
        }

        Station *next=NULL;

        if(busType == "SR"){
            if(direction == "forward"){
                next = curr->nextSR;
            }else{
                next = curr->prevSR;
            }
        }
        else if(busType == "ER"){
            Station *er=erHead;
            while(er && er->name != current){
                er=er->nextER;
            }
            if(er){
                if(direction == "forward"){
                    next=er->nextER;
                }else{
                    next=er->prevER;
                }
            }
            else{
                cout<<"This station is not on the Express Route."<<endl;
                return;
            }
        }

        if(next){
            cout<<"Next station("<<busType<<"): "<<next->name<<endl;
        }else{
            cout<<"End of the "<<busType<<" route reached."<<endl;
        }
    }

    void meetUpSimulation(User A, User B){
        cout<<"\n=== Meetup Simulation ==="<<endl;
        Station *A_curr=(A.busType == "SR") ? srHead : erHead;
        Station *B_curr=(B.busType == "SR") ? srHead : erHead;

        while(A_curr && A_curr->name != A.currentStation){
            A_curr=(A.busType == "SR") ? A_curr->nextSR : A_curr->nextER;
        }
        while(B_curr && B_curr->name != B.currentStation){
            B_curr=(B.busType == "SR") ? B_curr->nextSR : B_curr->nextER;
        }
        if(!A_curr || !B_curr){
            cout<<"Invalid start station."<<endl;
            return;
        }

        int timeA = A.startTime;
        int timeB = B.startTime;
        int endTimeSec = 22*3600;

        while(timeA <= endTimeSec && timeB <= endTimeSec && A_curr && B_curr){
            if(A_curr->name == B_curr->name && timeA == timeB){
                cout<<" "<<A.name<<" and "<<B.name<<" meet at "
                    <<A_curr->name<<" at "<<timeA / 3600<<":"
                    <<(timeA % 3600) / 60<<""<<endl;
                return;
            }
            A_curr=(A.direction == "forward")
                         ?((A.busType == "SR") ? A_curr->nextSR : A_curr->nextER)
                         :((A.busType == "SR") ? A_curr->prevSR : A_curr->prevER);

            B_curr=(B.direction == "forward")
                         ?((B.busType == "SR") ? B_curr->nextSR : B_curr->nextER)
                         :((B.busType == "SR") ? B_curr->prevSR : B_curr->prevER);

            timeA += 137;
            timeB += 137;
        }
        cout<<"They never meet."<<endl;
    }
};

int main(){
    BRTManagementsystem brt;
    string choice;
    do
{
        cout<<"\n========================================================"<<endl;
        cout<<"         Peshawar BRT Network Management System"<<endl;
        cout<<"========================================================"<<endl;
        cout<<"[1] Add Station to Route"<<endl;
        cout<<"[2] Remove Station from Route"<<endl;
        cout<<"[3] Display Full Map(SR-02)"<<endl;
        cout<<"[4] Display Express Map(ER-01)"<<endl;
        cout<<"[5] Calculate Fare(SR/ER)"<<endl;
        cout<<"[6] Calculate Travel Time(SR/ER)"<<endl;
        cout<<"[7] Find Best Route(Compare SR vs ER)"<<endl;
        cout<<"[8] Reverse Route(Chamkani <-> Karkhano)"<<endl;
        cout<<"[9] Show Next Station(given current + direction)"<<endl;
        cout<<"[10] Meet-Up Simulation(Two Users)"<<endl;
        cout<<"--------------------------------------------------------"<<endl;
        cout<<"[q] Quit Application"<<endl;
        cout<<"========================================================"<<endl;
        cout<<"Enter choice: ";
        cin >> choice;

        if(choice == "1"){
            int id;
            string name;
            cout<<"Enter Station ID: ";
            cin >> id;
            cout<<"Enter Station Name: ";
            cin.ignore();
            getline(cin, name);
            brt.addStation(id, name);
        }
        else if(choice == "2"){
            int id;
            cout<<"Enter Station ID to remove: ";
            cin >> id;
            brt.removeStation(id);
        }
        else if(choice == "3"){
            brt.displayFullMap();
        }
        else if(choice == "4"){
            brt.displayExpressMap();
        }
        else if(choice == "5"){
            string from, to, type;
            cout<<"Enter From Station: ";
            cin.ignore();
            getline(cin, from);
            cout<<"Enter To Station: ";
            getline(cin, to);
            cout<<"Enter Bus Type(SR/ER): ";
            cin >> type;
            brt.calculateFare(from, to, type);
        }
        else if(choice == "6"){
            string from, to, type;
            cout<<"Enter From Station: ";
            cin.ignore();
            getline(cin, from);
            cout<<"Enter To Station: ";
            getline(cin, to);
            cout<<"Enter Bus Type(SR/ER): ";
            cin >> type;
            brt.calculateTravelTime(from, to, type);
        }
        else if(choice == "7"){
            string from, to;
            cout<<"Enter From Station: ";
            cin.ignore();
            getline(cin, from);
            cout<<"Enter To Station: ";
            getline(cin, to);
            brt.findBestRoute(from, to);
        }
        else if(choice == "8"){
            string type;
            cout<<"Enter Bus Type to Reverse(SR/ER): ";
            cin >> type;
            brt.reverseRoute(type);
        }
        else if(choice == "9"){
            string current, type, direction;
            cout<<"Enter Current Station: ";
            cin.ignore();
            getline(cin, current);
            cout<<"Enter Bus Type(SR/ER): ";
            cin >> type;
            cout<<"Enter Direction(forward/backward): ";
            cin >> direction;
            brt.showNextStation(current, type, direction);
        }
        else if(choice == "10"){
            BRTManagementsystem::User A, B;
            cout<<"--- User A ---"<<endl;
            cout<<"Name: ";
            cin.ignore();
            getline(cin, A.name);
            cout<<"Start Station: ";
            getline(cin, A.currentStation);
            cout<<"Bus Type(SR/ER): ";
            cin >> A.busType;
            cout<<"Direction(forward/backward): ";
            cin >> A.direction;
            cout<<"Start Time(in seconds after 6:00 AM): ";
            cin >> A.startTime;

            cout<<"\n--- User B ---"<<endl;
            cin.ignore();
            cout<<"Name: ";
            getline(cin, B.name);
            cout<<"Start Station: ";
            getline(cin, B.currentStation);
            cout<<"Bus Type(SR/ER): ";
            cin >> B.busType;
            cout<<"Direction(forward/backward): ";
            cin >> B.direction;
            cout<<"Start Time(in seconds after 6:00 AM): ";
            cin >> B.startTime;

            brt.meetUpSimulation(A, B);
        }
        else if(choice == "q" || choice == "Q"){
            cout<<"\nThank you for using Peshawar BRT System!"<<endl;
        }
        else{
            cout<<"Invalid choice. Try again!"<<endl;
        }

    } while(choice != "q" && choice != "Q");

    return 0;
}
