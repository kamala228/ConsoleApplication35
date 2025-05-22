#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <windows.h>

using namespace std;

template<typename T>
class Stage {
private:
    queue<T> q;
    mutex m;
    condition_variable cv;

public:
    void push(T item) {
        unique_lock<mutex> lock(m);
        q.push(item);
        cv.notify_one();
    }

    T pop() {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this] { return !q.empty(); });
        T item = q.front();
        q.pop();
        return item;
    }
};

struct Wood {};
struct Log {};
struct Plank {};
struct WoodProduct {};

struct Stone {};
struct Block {};
struct Tile {};
struct StoneProduct {};

struct IronOre {};
struct Ingot {};
struct Plate {};
struct IronProduct {};

struct FinalProduct {
    WoodProduct w;
    StoneProduct s;
    IronProduct i;
};

int main() {
    system("chcp 1251 >nul");

    Stage<Wood> woodStage;
    Stage<Log> logStage;
    Stage<Plank> plankStage;
    Stage<WoodProduct> woodProductStage;

    Stage<Stone> stoneStage;
    Stage<Block> blockStage;
    Stage<Tile> tileStage;
    Stage<StoneProduct> stoneProductStage;

    Stage<IronOre> ironStage;
    Stage<Ingot> ingotStage;
    Stage<Plate> plateStage;
    Stage<IronProduct> ironProductStage;

    thread t1([&] {
        woodStage.push(Wood{});
        cout << "Дерево здобуто\n";
        });

    thread t2([&] {
        Wood w = woodStage.pop();
        logStage.push(Log{});
        cout << "Колода зроблена\n";
        });

    thread t3([&] {
        Log l = logStage.pop();
        plankStage.push(Plank{});
        cout << "Дошка зроблена\n";
        });

    thread t4([&] {
        Plank p = plankStage.pop();
        woodProductStage.push(WoodProduct{});
        cout << "Дерев’яний продукт готовий\n";
        });

    thread t5([&] {
        stoneStage.push(Stone{});
        cout << "Камінь здобуто\n";
        });

    thread t6([&] {
        Stone s = stoneStage.pop();
        blockStage.push(Block{});
        cout << "Блок зроблений\n";
        });

    thread t7([&] {
        Block b = blockStage.pop();
        tileStage.push(Tile{});
        cout << "Плита зроблена\n";
        });

    thread t8([&] {
        Tile t = tileStage.pop();
        stoneProductStage.push(StoneProduct{});
        cout << "Кам’яний продукт готовий\n";
        });

    thread t9([&] {
        ironStage.push(IronOre{});
        cout << "Залізо добуто\n";
        });

    thread t10([&] {
        IronOre i = ironStage.pop();
        ingotStage.push(Ingot{});
        cout << "Злиток зроблений\n";
        });

    thread t11([&] {
        Ingot in = ingotStage.pop();
        plateStage.push(Plate{});
        cout << "Пластина зроблена\n";
        });

    thread t12([&] {
        Plate pl = plateStage.pop();
        ironProductStage.push(IronProduct{});
        cout << "Залізний продукт готовий\n";
        });

    thread t13([&] {
        WoodProduct w = woodProductStage.pop();
        StoneProduct s = stoneProductStage.pop();
        IronProduct i = ironProductStage.pop();
        FinalProduct f{ w, s, i };
        cout << "Фінальний продукт створено!\n";
        });

    t1.join(); t2.join(); t3.join(); t4.join();
    t5.join(); t6.join(); t7.join(); t8.join();
    t9.join(); t10.join(); t11.join(); t12.join();
    t13.join();

    return 0;
}
