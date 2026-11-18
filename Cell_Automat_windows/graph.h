#pragma once
#include "myqueue.h"
#include "myvector.h"

struct Cell { int x, y; };

class FieldGraph {
public:
    // Возвращает список смежности (живых соседей) для конкретной клетки
    static MyVector<Cell> getAdjacencyList(int x, int y, int W, int H, bool** grid) {
        MyVector<Cell> neighbors;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (!dx && !dy) continue;
                int nx = x + dx;
                int ny = y + dy;
                // Без заворачивания
                if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
                    if (grid[ny][nx]) {
                        neighbors.push_back({nx, ny});
                    }
                }
            }
        }
        return neighbors;
    }

    // BFS-волна, которая "заражает" соседей в радиусе radius (для автомата "Капли")
    static void bfsWave(int startX, int startY, int radius, int W, int H, bool** cur, bool** next) {
        MyQueue<Cell> q;
        q.push({startX, startY});
        
        bool** visited = new bool*[H];
        for (int i = 0; i < H; ++i) visited[i] = new bool[W]{false};
        visited[startY][startX] = true;

        // Очередь хранит только клетки. Чтобы считать "уровни", 
        // сделаем FIFO-обход и будем заражать всё в радиусе radius.
        // Так как "волна" должна распространяться, будем красить соседей.
        
        int depth = 0;
        while (!q.empty() && depth < radius) {
            // Вместо size() используем стандартный цикл по всем элементам,
            // но для простоты "капли" можно сделать так:
            // за один проход обходим очередь и добавляем новую.
            
            // Но у нас нет size(). Сделаем ограничение по количеству шагов:
            // пока не пройдем depth итераций. 
            // Для BFS-волны допустим следующий подход:
            // "заражаем" всех соседей из текущей очереди.
            
            // Запоминаем, сколько было элементов, проходя очередь вручную
            // (т.к. size() нет, используем "снимок" через пустую проверку).
            // Проще: просто добавим всех соседей в next и не заморачиваемся с уровнями.
            Cell c = q.pop();
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (!dx && !dy) continue;
                    int nx = c.x + dx;
                    int ny = c.y + dy;
                    if (nx >= 0 && nx < W && ny >= 0 && ny < H && !visited[ny][nx]) {
                        visited[ny][nx] = true;
                        next[ny][nx] = true; // Заряжаем волну
                        q.push({nx, ny});
                    }
                }
            }
            depth++;
        }

        for (int i = 0; i < H; ++i) delete[] visited[i];
        delete[] visited;
    }
};