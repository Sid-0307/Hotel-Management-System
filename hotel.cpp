#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;


struct Room {
    int rno;
    string type;
    int capacity;
    string status;
    int rent_per_day;
    string name;
    string phone;
};

class Admin {
private:
    mysql::MySQL_Driver *driver;
    Connection *con;

public:
    Admin() {
        // Initialize driver and connection
        driver = mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "admin", "admin");
        con->setSchema("hotelDB");
    }

    ~Admin() {
        delete con;
    }

    // Function to add room
    void addRoom() {
        Room newRoom;
        cout << "\n\n🟧 🟧 🟧    Add Room    🟧 🟧 🟧\nEnter Room Number: ";
        cin >> newRoom.rno;
        cout << "Enter Room Type: ";
        cin >> newRoom.type;
        cout << "Enter Room Capacity: ";
        cin >> newRoom.capacity;
        newRoom.status = "vacant";
        cout << "Enter Rent per Day: ";
        cin >> newRoom.rent_per_day;

        PreparedStatement *prep_stmt = con->prepareStatement("INSERT INTO rooms (rno, type, capacity, status, rent_per_day,customer_name,customer_phone) VALUES (?, ?, ?, ?, ?,?,?)");
        prep_stmt->setInt(1, newRoom.rno);
        prep_stmt->setString(2, newRoom.type);
        prep_stmt->setInt(3, newRoom.capacity);
        prep_stmt->setString(4, newRoom.status);
        prep_stmt->setInt(5, newRoom.rent_per_day);
        prep_stmt->setString(6,"");
        prep_stmt->setString(7,"");
        prep_stmt->executeUpdate();
        delete prep_stmt;

        cout << "🟩 🟩 🟩  Room added successfully! 🟩 🟩 🟩\n";
    }

    // Function to modify an existing room
    void modifyRoom() {
        int rno;
        Room newRoom;
        cout << "\n\n🟧 🟧 🟧     Modify Room    🟧 🟧 🟧 \nEnter Room Number to Modify: ";
        cin >> rno;

        Statement *stmt = con->createStatement();
        ResultSet *res = stmt->executeQuery("SELECT * FROM rooms WHERE rno = " + to_string(rno));

        if (res->next()) {
            cout << "Enter New Room Type: ";
            cin >> newRoom.type;
            cout << "Enter New Room Capacity: ";
            cin >> newRoom.capacity;
            cout << "Enter New Rent per Day: ";
            cin >> newRoom.rent_per_day;

            PreparedStatement *prep_stmt = con->prepareStatement("UPDATE rooms SET type=?, capacity=?, rent_per_day=? WHERE rno=?");
            prep_stmt->setString(1, newRoom.type);
            prep_stmt->setInt(2, newRoom.capacity);
            prep_stmt->setInt(3, newRoom.rent_per_day);
            prep_stmt->setInt(4, rno);
            prep_stmt->executeUpdate();
            delete prep_stmt;

            cout << "🟩 🟩 🟩  Room modified successfully! 🟩 🟩 🟩\n";
        } else {
            cout << "\n🟥 🟥 🟥  Room not found 🟥 🟥 🟥\n";
        }
        delete res;
        delete stmt;
    }

    // Function to delete an existing room
    void deleteRoom() {
        int rno;
        cout << "\n\n🟧 🟧 🟧      Delete Room     🟧 🟧 🟧 \nEnter Room Number to Delete: ";
        cin >> rno;

        Statement *stmt = con->createStatement();
        ResultSet *res = stmt->executeQuery("SELECT * FROM rooms WHERE rno = " + to_string(rno));

        if (res->next()) {
            PreparedStatement *prep_stmt = con->prepareStatement("DELETE FROM rooms WHERE rno=?");
            prep_stmt->setInt(1, rno);
            prep_stmt->executeUpdate();
            delete prep_stmt;
            cout << "🟩 🟩 🟩  Room deleted successfully! 🟩 🟩 🟩\n";
        } else {
            cout << "\n🟥 🟥 🟥  Room not found 🟥 🟥 🟥\n";
        }
        delete res;
        delete stmt;
    }

    // Function to display all rooms
    void displayAllRooms() {
        Statement *stmt = con->createStatement();
        ResultSet *res = stmt->executeQuery("SELECT * FROM rooms");
        cout << "\n\n🟧 🟧 🟧     Display All Rooms   🟧 🟧 🟧 ";
        if (res->rowsCount() == 0) {
            cout << "\n🟥 🟥 🟥  No rooms created 🟥 🟥 🟥\n\n";
        } else {
            cout << "\nRoomno\tType\tCapacity\tRent\tStatus\t\tName\tPhone";
            while (res->next()) {
                cout << "\n" << res->getInt("rno") << "\t" << res->getString("type") << "\t" << res->getInt("capacity") << "\t\t" << res->getInt("rent_per_day") << "\t" << res->getString("status") << "\t" << res->getString("customer_name") << "\t" << res->getString("customer_phone");
            }
        }
        delete res;
        delete stmt;
    }
};


class User {
private:
    mysql::MySQL_Driver *driver;
    Connection *con;

public:
    User() {
        driver = mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "admin", "admin");
        con->setSchema("hotelDB");
    }

    ~User() {
        delete con;
    }

    // Function to display available rooms
    void availableRooms() {
        int c = 0;
        cout << "\n\n🟧 🟧 🟧     Available Rooms    🟧 🟧 🟧 ";
        Statement *stmt = con->createStatement();
        ResultSet *res = stmt->executeQuery("SELECT * FROM rooms");
        if (res->rowsCount() == 0) {
            cout << "\n🟥 🟥 🟥  No rooms created 🟥 🟥 🟥\n\n";
        } 
        else 
        {
            Statement *stmt = con->createStatement();
            ResultSet *res = stmt->executeQuery("SELECT * FROM rooms WHERE status = 'vacant'");
            if(res->rowsCount()>=1){
                cout << "\nRoomno\tType\tCapacity\tRent";
                while (res->next()) 
                {
                    cout << "\n" << res->getInt("rno") << "\t" << res->getString("type") << "\t" << res->getInt("capacity") << "\t\t" << res->getInt("rent_per_day");
                }
            }
            else{
                cout << "\n🟥 🟥 🟥  No vacant rooms 🟥 🟥 🟥";
            }
        }
        delete res;
        delete stmt;
    }

    // Function to checkIn
    void checkIn() {
        int rno;
        cout << "\n\n\n🟧 🟧 🟧      Check In    🟧 🟧 🟧 \nEnter room number: ";
        cin >> rno;

        Statement *stmt = con->createStatement();
        ResultSet *res = stmt->executeQuery("SELECT * FROM rooms WHERE rno = " + to_string(rno));

        if (res->next() && res->getString("status") == "vacant") {
            string name,phone;
            PreparedStatement *prep_stmt = con->prepareStatement("UPDATE rooms SET status='Occupied', customer_name=?, customer_phone=? WHERE rno=?");
            cout << "Enter customer name: ";
            cin >> name;
            cout << "Enter customer phone: ";
            cin >> phone;

            prep_stmt->setString(1, name);
            prep_stmt->setString(2, phone);
            prep_stmt->setInt(3, rno);
            prep_stmt->executeUpdate();
            delete prep_stmt;

            cout << "\n🟩 🟩 🟩  Booking Successful 🟩 🟩 🟩\n\n";
        } 
        else if (res->next() && res->getString("status") == "Occupied") 
        {
            cout << "\n🟥 🟥 🟥  Room not vacant 🟥 🟥 🟥\n\n";
        }
        else 
        {
            cout << "\n🟥 🟥 🟥  No such room available 🟥 🟥 🟥\n\n";
        }

        delete res;
        delete stmt;
    }

    // Function to checkOut
    void checkOut() {
        int rno;
        cout << "\n\n🟧 🟧 🟧      Check Out   🟧 🟧 🟧 \nEnter room number: ";
        cin >> rno;

        Statement *stmt = con->createStatement();
        ResultSet *res = stmt->executeQuery("SELECT * FROM rooms WHERE rno = " + to_string(rno));

        if (res->next() && res->getString("status") == "Occupied") {
            PreparedStatement *prep_stmt = con->prepareStatement("UPDATE rooms SET status='vacant', customer_name='', customer_phone='' WHERE rno=?");
            prep_stmt->setInt(1, rno);
            prep_stmt->executeUpdate();
            delete prep_stmt;

            cout << "\n🟩 🟩 🟩  CheckOut Successful 🟩 🟩 🟩";
        } else {
            cout << "\n🟥 🟥 🟥  Invalid action 🟥 🟥 🟥";
        }
        delete res;
        delete stmt;
    }
};

int main(){
    int x,a;
    string pass;
    Admin admin;
    User user;
    cout<<"\nWelcome to Amaze Hotels\n";
    while(1){
        cout<<"\n\n🟧 🟧 🟧     Menu     🟧 🟧 🟧\n1.Available rooms\n2.CheckIn\n3.CheckOut\n4.Admin privileges\n5.Exit\nEnter your choice:";
        cin>>x;
        if(x==5){
            cout<<"\n\nThank you for choosing Amaze Hotels, Have a wonderful day :)\n";
            break;
        }
        switch(x){
            case 1:
                user.availableRooms();
                break;
            case 2:
                user.checkIn();
                break;
            case 3:
                user.checkOut();
                break;
            case 4:
                cout<<"Enter password";
                cin>>pass;
                if(pass=="admin"){
                    while(1){
                        cout<<"\n\n🟧 🟧 🟧      Admin Menu     🟧 🟧 🟧\n1.Add Rooms\n2.Modify Rooms\n3.Delete Rooms\n4.Display All Rooms\n5.Exit\nEnter your choice:";
                        cin>>a;

                        if(a==5){
                            break;
                        }

                        switch (a) {
                            case 1:
                                admin.addRoom();
                                break;
                            case 2:
                                admin.modifyRoom();
                                break;
                            case 3:
                                admin.deleteRoom();
                                break;
                            case 4:
                                admin.displayAllRooms();
                                break;
                            default:
                                cout<<"⚠️ ⚠️ ⚠️  Please enter a valid choice ⚠️ ⚠️ ⚠️";
                        }

                    }
                    
                }
                else{
                    cout<<"⚠️ ⚠️ ⚠️  Authorization Failed ⚠️ ⚠️ ⚠️";
                }
                break;
            default:
                cout<<"⚠️ ⚠️ ⚠️  Please enter a valid choice ⚠️ ⚠️ ⚠️";
    }
    }
    return 0;
}