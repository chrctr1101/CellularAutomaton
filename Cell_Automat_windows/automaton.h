#pragma once
#include "graph.h"
#include "myvector.h"

class Automaton {
public:
    virtual ~Automaton() {}
    virtual void step(bool** cur, bool** next, int W, int H) = 0;
    virtual void reset() {}
    virtual void onCellAdded(int x, int y) {}
    virtual void onCellRemoved(int x, int y) {}
};

class LifeAutomaton : public Automaton {
public:
    void step(bool** cur, bool** next, int W, int H) override {
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                int n = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (!dx && !dy) continue;
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < W && ny >= 0 && ny < H && cur[ny][nx]) n++;
                    }
                }
                if (cur[y][x]) next[y][x] = (n == 2 || n == 3);
                else next[y][x] = (n == 3);
            }
        }
    }
};

// ==================== КАПЛИ (Множественные волны) ====================
struct Wave {
    int cx, cy;       // Центр волны
    int birthTime;    // Время рождения
};

class DropsAutomaton : public Automaton {
    MyVector<Wave> waves;
    int currentTime;
    const int LIFETIME = 3;   // Сколько шагов клетка светится после прохождения фронта
    const int MAX_RADIUS = 50; // Максимальный радиус волны (чтобы не заполнить всё)

public:
    DropsAutomaton() : currentTime(0) {}

    void reset() override {
        waves.clear();
        currentTime = 0;
    }

    void onCellAdded(int x, int y) override {
        // Каждый клик создает новую волну
        Wave w;
        w.cx = x;
        w.cy = y;
        w.birthTime = currentTime;
        waves.push_back(w);
    }

    void onCellRemoved(int x, int y) override {
        // Удаляем волны, центр которых совпадает с удаленной клеткой
        MyVector<Wave> newWaves;
        for (size_t i = 0; i < waves.size(); ++i) {
            if (waves[i].cx != x || waves[i].cy != y) {
                newWaves.push_back(waves[i]);
            }
        }
        waves = newWaves;
    }

    void step(bool** cur, bool** next, int W, int H) override {
        currentTime++;

        // Очищаем next
        for (int y = 0; y < H; ++y)
            for (int x = 0; x < W; ++x)
                next[y][x] = false;

        // Для каждой волны вычисляем её текущее состояние
        for (size_t i = 0; i < waves.size(); ++i) {
            Wave& w = waves[i];
            int age = currentTime - w.birthTime;

            // Волна распространяется на расстояние age (в манхэттенской метрике)
            // Рисуем "кольцо" от (age - LIFETIME) до age
            int innerRadius = age - LIFETIME;
            if (innerRadius < 0) innerRadius = 0;

            for (int y = 0; y < H; ++y) {
                for (int x = 0; x < W; ++x) {
                    int dist = abs(x - w.cx) + abs(y - w.cy);
                    if (dist >= innerRadius && dist <= age && dist <= MAX_RADIUS) {
                        next[y][x] = true;
                    }
                }
            }
        }

        // Удаляем волны, которые ушли за границу поля или превысили MAX_RADIUS
        MyVector<Wave> aliveWaves;
        for (size_t i = 0; i < waves.size(); ++i) {
            Wave& w = waves[i];
            int age = currentTime - w.birthTime;
            if (age <= MAX_RADIUS + LIFETIME) {
                aliveWaves.push_back(w);
            }
        }
        waves = aliveWaves;
    }
};

class CustomAutomaton : public Automaton {
    unsigned int birthMask, surviveMask;
public:
    CustomAutomaton(unsigned int b, unsigned int s) : birthMask(b), surviveMask(s) {}
    void step(bool** cur, bool** next, int W, int H) override {
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                int n = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (!dx && !dy) continue;
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < W && ny >= 0 && ny < H && cur[ny][nx]) n++;
                    }
                }
                if (cur[y][x]) next[y][x] = (surviveMask >> n) & 1;
                else next[y][x] = (birthMask >> n) & 1;
            }
        }
    }
};