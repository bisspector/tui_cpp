#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <string>
#include "execution.h"
#include <crow.h>
/////

using namespace std;
using namespace crow;

std::map<string, bool> used;

void start() {
    int rc = sqlite3_open("./restaurant.db", &db);
    if (rc) {
        cout << "Can't open database: \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Opened database successfully\n";
    }
    createTable("menu", {"id", "name", "price", "time", "description", "image"}, {"intkeynn", "stringnn", "intnn", "intnn", "stringnn", "stringnn"});
    createTable("users", {"login", "passwHash"}, {"stringnn", "stringnn"});
    createTable("storage", {"serialId", "prodId", "abound"}, {"intkeynn", "intnn", "intnn"});
}


std :: vector <std :: string> DishVec;
std :: vector <std :: string> DishArray;

std :: string MinusSpace(std :: string s){
    while (s[0] == ' '){
        s.erase(0, 1);
    }
    return s;
}

int Min(int a, int b){
    if (a <= b)
        return a;
    return b;
}

void Convert_to_json(std :: string dish0, std :: string dish, std :: string price0, std :: string price, std :: string image0, std :: string image, std :: string description0, std :: string description, std :: string time0, std :: string time){
    std :: string rez = "{ \"" + dish0 + "\":\"" + dish + "\", \"" + price0 + "\":\"" + price + "\", \"" + image0 + "\":\"" + image  + "\", \"" + description0 + "\":\"" + description + "\", \"" + time0 + "\":\"" + time + "\" }" ;
    std :: cout << "\n" << rez;
    if (!used[rez])
        DishVec.push_back(rez);
    used[rez] = true;
}

void Convert_to_comand(std :: string s){
    s += '_';
    std :: string name = "";
    std :: string price = "";
    std :: string time = "";
    std :: string description = "";
    std :: string image = "./images/";
    int top = 0, front = 0;
    for (int i = 0; i < s.size(); i++){
        if (s[i] != '_'){
            if (s[i] == '-')
                s[i] = ' ';
        } else {
            top++;
            cout << "\n\n\n" << top << "\n\n\n";
            if (top == 1){
                name = s.substr(front, i - front);
            }
            if (top == 2){
                price = s.substr(front, i - front);
            }
            if (top == 3){
                time = s.substr(front, i - front);
            }
            if (top == 4){
                description = s.substr(front, i - front);
            }
            if (top == 5){
                image += s.substr(front, i - front) + ".png";
            }
            front = i + 1;
        }
    }
    Convert_to_json("name", name, "price", price, "image", image, "description", description, "time", time);
}

/*    Тут будут написаны некоторые примеры                           */
/*    add("menu", {"1", "pizza", "23", "1"});                        */
/*    update("menu", "id", "1", {"1", "soupe", "13", str(1)})        */
/*    delete("menu", "id", "1")                                      */
/*    take("menu", "id", "3", "price") - взять по id цену            */
/*-------------------------------------------------------------------*/

int main() {

    std :: string AddMeal = "";
    bool need;
    
    


    crow::App<> app;

    ///


    

    CROW_ROUTE(app, "/add/<string>")
        .methods("GET"_method, "POST"_method, "DELETE"_method)
        ([&AddMeal, &need](const request& req, const string& id) {
            if (req.method == "GET"_method)
            {
                if ((req.url_params.get("v") != nullptr) & (req.url_params.get("q") != nullptr))
                {
                    // ...
                }
                if (AddMeal != id){
                    need = true;
                }
                if (AddMeal == id){
                    need = false;
                }
                AddMeal = id;
                if (need == true){
                    cout << "\n\n\n\n\n" << AddMeal << "\n\n\n\n\n";
                    Convert_to_comand(AddMeal);
                }

                return response(200, "You used GET");
            }
            else if (req.method == "POST"_method)
            {
                return response(200, "You used POST");
            }
            else if (req.method == "DELETE"_method)
            {
                return response(200, "You used DELETE");
            }
            else
            {
                return response(404);
            }
        });
    ///

    CROW_ROUTE(app, "/menu")([](const crow::request&, crow::response& res) -> void{
        res.add_header("Access-Control-Allow-Origin", "*");

        start();
        // add("menu", {"1", "pizza", "23", "1", "tasty thing", "../image/png1.png"});
        // int ColAsk = 2;
        //dropTable("menu");
        //add("menu", {"1", "pizza", "23", "1", "tasty thing", "../image/png1.png"});
        //int ColAsk = 2;
        string name = "";
        string price = "";
        string time = "";
        string description = "";
        string image = "./image/";
        // name = take("menu", "id", "1", "price");
        // price = take("menu", "id", "1", "name");
        // time = take("menu", "id", "1", "time");
        // description = take("menu", "id", "1", "description");
        // image = take("menu", "id", "1", "image");
        cout << name << ' ' << price << ' ' << time << ' ' << description << ' ' << image << '\n';

        res.write("[ ");
        for (int i = 0; i < DishVec.size(); i++){
            // Convert_to_json("name", name, "price", price, "image", image, "description", description, "time", time);
            res.write(DishVec[i]);
            if (i != DishVec.size() - 1){
                res.write(", ");
            }
        }

        
        res.write(" ]");
        res.end();
    });

    app.port(18080)
        .multithreaded()
        .run();

    //dropTable("menu");
    return 0;
}