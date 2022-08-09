#include<iostream>
#include<vector>
#include<list>
#include<SFML/Graphics.hpp>
#include<time.h>
#include<set>
using namespace std;
using namespace sf;

float interval=0.3;
float cakeinterval=0.3;
const int W=30;
const int H=30;
vector<vector<int>> cake(H,vector<int>(W,0));
const int size_=30;
const int gap=1;
struct Point{
    Point(int x,int y):x(x),y(y){}
    bool operator<(const Point& p)const{
        if(x<p.x||x==p.x&&y<p.y){
            return true;
        }
        return false;
    }
    int x;
    int y;
};
void cakeShow(RenderWindow& window){
    for(int i=0;i<H;i++){
        for(int j=0;j<W;j++){
            if(cake[i][j]){
                RectangleShape rec;
                rec.setSize(Vector2f(size_-2,size_-2));
                rec.setPosition(Vector2f(j*size_+1,i*size_+1));
                rec.setFillColor(Color::Green);
                window.draw(rec);
            }
        }
    }
}
class Snake{
    public:
        Snake(RenderWindow& wind):window(wind){
            body.push_back({15,15});
            body.push_back({15,16});
            body.push_back({15,17});
            body.push_back({15,18});
            body.push_back({15,19});
            st.insert({15,15});
            st.insert({15,16});
            st.insert({15,17});
            st.insert({15,18});
            st.insert({15,19});
        }
        void eat();
        void move(int);
        void move(pair<int,int> dir);
        void display();
        void left();
        void up();
        void down();
        void right();
        void gameOver();
        bool collision(Point& p);
        void genCake();
    private:
        RenderWindow& window;
        set<Point> st;
        set<Point> cakest;
        list<Point> body;
        int cakeNum=0;
};
void Snake::genCake(){
    if(cakeNum<6){
        int w=rand()%W;
        int h=rand()%H;
        if(st.find({w,h})==st.end()){
            cakeNum++;
            cakest.insert({w,h});
            cake[w][h]=1;
        }
    }
}
void Snake::move(int direction){
    switch(direction){
        case 1:
            move({-1,0});
            break;
        case 2:
            move({0,1});
            break;
        case 3:
            move({1,0});
            break;
        case 4:
            move({0,-1});
            break;
    }
}
void Snake::move(pair<int,int> dir){
    auto p=body.front();
    Point newhead(p.x+dir.first,p.y+dir.second);
    if(collision(newhead)){
        gameOver();
        return;
    }
    if(cake[newhead.x][newhead.y]){
        cakeNum--;
        cakest.erase({newhead.x,newhead.y});
        cake[newhead.x][newhead.y]=0;
    }else{
        st.erase(body.back());
        body.pop_back();
    }
    st.insert(newhead);
    body.push_front(newhead);
}
void Snake::gameOver(){
    cout<<"gameOver"<<endl;
    window.close();
}
void Snake::display(){
    window.clear();
    RectangleShape rec;
    for(auto it:body){
        rec.setSize(Vector2f(size_-2,size_-2));
        rec.setPosition(Vector2f(it.x*size_+1,it.y*size_+1));
        rec.setFillColor(Color::Red);
        window.draw(rec);
    }
    for(auto it:cakest){
        rec.setSize(Vector2f(size_-2,size_-2));
        rec.setPosition(Vector2f(it.x*size_+1,it.y*size_+1));
        rec.setFillColor(Color::Green);
        window.draw(rec);
    }
    window.display();
}
bool Snake::collision(Point& head){
    if(head.x<0||head.x>=W||head.y<0||head.y>=H||st.find(head)!=st.end()){
        return true;
    }
    return false;
}

int main(){
    srand(time(0));
    RenderWindow window(VideoMode(900,900),"Snake Game");
    Snake snake(window);
    float timer=0;
    float caketimer=0;
    float speedtimer=0;
    Clock clock;
    int direction=1;
    while(window.isOpen()){
        Event e;
        float tem=clock.getElapsedTime().asSeconds();
        timer+=tem;
        caketimer+=tem;
        speedtimer+=tem;
        clock.restart();
        while(window.pollEvent(e)){
            switch(e.type){
                case Event::Closed:
                    window.close();
                    break;
                case Event::KeyPressed:
                    //检查direction不能开倒车
                    if((e.key.code==Keyboard::A||e.key.code==Keyboard::Left)&&direction!=3){
                        direction=1;
                    }else if((e.key.code==Keyboard::S||e.key.code==Keyboard::Down)&&direction!=4){
                        direction=2;
                    }else if((e.key.code==Keyboard::D||e.key.code==Keyboard::Right)&&direction!=1){
                        direction=3;
                    }else if((e.key.code==Keyboard::W||e.key.code==Keyboard::Up)&&direction!=2){
                        direction=4;
                    }
                    sleep(seconds(interval/4));
                    snake.move(direction);
                    timer=0;
                    break;
            }
        }
        if(timer>interval){
            timer=0;
            snake.move(direction);
        }
        if(caketimer>cakeinterval){
            cakeinterval=rand()%40*1.0/10+0.1;
            caketimer=0;
            snake.genCake();
        }
        if(speedtimer>5&&interval>0.05){
            cout<<interval<<endl;
            speedtimer=0;
            interval-=0.01;
        }
        snake.display();
    }
    return 0;
}