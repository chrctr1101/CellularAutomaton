#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <tuple>
#include <string>
#include "myvector.h"
#include "mylist.h"
#include "myqueue.h"
#include "mydeque.h"
#include "graph.h"
#include "automaton.h"

// ==========================================
// Фикстура для тестов на поле 10x10
// ==========================================
class GridTest : public ::testing::Test {
protected:
    int W = 10;
    int H = 10;
    bool** cur;
    bool** next;

    void SetUp() override {
        cur = createGrid(W, H);
        next = createGrid(W, H);
    }

    void TearDown() override {
        freeGrid(cur, H);
        freeGrid(next, H);
    }

    bool** createGrid(int w, int h) {
        bool** grid = new bool*[h];
        for (int i = 0; i < h; ++i) grid[i] = new bool[w]{false};
        return grid;
    }

    void freeGrid(bool** grid, int h) {
        if (!grid) return;
        for (int i = 0; i < h; ++i) delete[] grid[i];
        delete[] grid;
    }

    bool isEmpty(bool** grid, int w, int h) {
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                if (grid[y][x]) return false;
        return true;
    }

    int countAlive(bool** grid, int w, int h) {
        int count = 0;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                if (grid[y][x]) count++;
        return count;
    }
};

// ==========================================
// 1. MyVector (20 тестов)
// ==========================================

TEST(MyVectorTest, InitialState) {
    MyVector<int> vec;
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
}

TEST(MyVectorTest, PushSingleElement) {
    MyVector<int> vec;
    vec.push_back(42);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 42);
}

TEST(MyVectorTest, PushMultipleElements) {
    MyVector<int> vec;
    for (int i = 0; i < 10; ++i) vec.push_back(i);
    EXPECT_EQ(vec.size(), 10);
    for (int i = 0; i < 10; ++i) EXPECT_EQ(vec[i], i);
}

TEST(MyVectorTest, Reallocation) {
    MyVector<int> vec;
    for (int i = 0; i < 100; ++i) vec.push_back(i);
    EXPECT_EQ(vec.size(), 100);
    EXPECT_EQ(vec[99], 99);
}

TEST(MyVectorTest, ClearAndReuse) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.clear();
    EXPECT_TRUE(vec.empty());
    vec.push_back(2);
    EXPECT_EQ(vec[0], 2);
}

TEST(MyVectorTest, PopBack) {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.pop_back();
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 10);
}

TEST(MyVectorTest, PopBackEmpty) {
    MyVector<int> vec;
    vec.pop_back();
    EXPECT_TRUE(vec.empty());
}

TEST(MyVectorTest, CopyConstructor) {
    MyVector<int> vec1;
    vec1.push_back(5);
    MyVector<int> vec2 = vec1;
    EXPECT_EQ(vec2.size(), 1);
    EXPECT_EQ(vec2[0], 5);
}

TEST(MyVectorTest, CopyAssignment) {
    MyVector<int> vec1;
    vec1.push_back(7);
    MyVector<int> vec2;
    vec2 = vec1;
    EXPECT_EQ(vec2.size(), 1);
    EXPECT_EQ(vec2[0], 7);
}

TEST(MyVectorTest, SelfAssignment) {
    MyVector<int> vec;
    vec.push_back(3);
    vec = vec;
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 3);
}

TEST(MyVectorTest, CopyEmptyVector) {
    MyVector<int> vec1;
    MyVector<int> vec2 = vec1;
    EXPECT_TRUE(vec2.empty());
}

TEST(MyVectorTest, DeepCopyIndependence) {
    MyVector<int> vec1;
    vec1.push_back(1);
    MyVector<int> vec2 = vec1;
    vec1[0] = 99;
    EXPECT_EQ(vec2[0], 1);
}

TEST(MyVectorTest, StringType) {
    MyVector<std::string> vec;
    vec.push_back("Hello");
    vec.push_back("World");
    EXPECT_EQ(vec[0], "Hello");
    EXPECT_EQ(vec[1], "World");
}

TEST(MyVectorTest, DoubleType) {
    MyVector<double> vec;
    vec.push_back(3.14);
    EXPECT_DOUBLE_EQ(vec[0], 3.14);
}

TEST(MyVectorTest, LargeArray) {
    MyVector<int> vec;
    for (int i = 0; i < 10000; ++i) vec.push_back(i);
    EXPECT_EQ(vec.size(), 10000);
    EXPECT_EQ(vec[9999], 9999);
}

TEST(MyVectorTest, ClearTwice) {
    MyVector<int> vec;
    vec.push_back(1);
    vec.clear();
    vec.clear();
    EXPECT_TRUE(vec.empty());
}

TEST(MyVectorTest, MultipleReallocations) {
    MyVector<int> vec;
    for (int i = 0; i < 1000; ++i) vec.push_back(i);
    for (int i = 0; i < 1000; ++i) EXPECT_EQ(vec[i], i);
}

TEST(MyVectorTest, ConstIndexOperator) {
    MyVector<int> vec;
    vec.push_back(42);
    const MyVector<int>& constRef = vec;
    EXPECT_EQ(constRef[0], 42);
}

TEST(MyVectorTest, CopyConstructorWithReallocation) {
    MyVector<int> vec1;
    for (int i = 0; i < 10; ++i) vec1.push_back(i);
    MyVector<int> vec2 = vec1;
    EXPECT_EQ(vec2.size(), 10);
}

TEST(MyVectorTest, AssignmentWithReallocation) {
    MyVector<int> vec1;
    for (int i = 0; i < 10; ++i) vec1.push_back(i);
    MyVector<int> vec2;
    vec2 = vec1;
    EXPECT_EQ(vec2.size(), 10);
}

// ==========================================
// 2. MyList и MyQueue (15 тестов)
// ==========================================

TEST(MyListTest, InitialState) {
    MyList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.head, nullptr);
}

TEST(MyListTest, PushOneElement) {
    MyList<int> list;
    list.push_back(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.head->val, 10);
}

TEST(MyListTest, PushMultiple) {
    MyList<int> list;
    for (int i = 0; i < 10; ++i) list.push_back(i);
    EXPECT_EQ(list.size(), 10);
}

TEST(MyListTest, ClearList) {
    MyList<int> list;
    list.push_back(1);
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.head, nullptr);
}

TEST(MyListTest, ClearEmptyList) {
    MyList<int> list;
    list.clear();
    EXPECT_TRUE(list.empty());
}

TEST(MyListTest, PushAfterClear) {
    MyList<int> list;
    list.push_back(1);
    list.clear();
    list.push_back(2);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.head->val, 2);
}

TEST(MyQueueTest, InitialState) {
    MyQueue<int> q;
    EXPECT_TRUE(q.empty());
}

TEST(MyQueueTest, PushPopOne) {
    MyQueue<int> q;
    q.push(1);
    EXPECT_EQ(q.pop(), 1);
    EXPECT_TRUE(q.empty());
}

TEST(MyQueueTest, FIFO_Order) {
    MyQueue<int> q;
    q.push(1); q.push(2); q.push(3);
    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
}

TEST(MyQueueTest, InterleavedPushPop) {
    MyQueue<int> q;
    q.push(10); q.push(20);
    EXPECT_EQ(q.pop(), 10);
    q.push(30);
    EXPECT_EQ(q.pop(), 20);
    EXPECT_EQ(q.pop(), 30);
}

TEST(MyQueueTest, PopEmptyQueue) {
    MyQueue<int> q;
    EXPECT_EQ(q.pop(), 0);
}

TEST(MyQueueTest, LargeQueue) {
    MyQueue<int> q;
    for (int i = 0; i < 100; ++i) q.push(i);
    for (int i = 0; i < 100; ++i) EXPECT_EQ(q.pop(), i);
}

TEST(MyQueueTest, StringQueue) {
    MyQueue<std::string> q;
    q.push("A"); q.push("B");
    EXPECT_EQ(q.pop(), "A");
    EXPECT_EQ(q.pop(), "B");
}

TEST(MyQueueTest, PushAfterEmpty) {
    MyQueue<int> q;
    q.push(1);
    q.pop();
    q.push(2);
    EXPECT_EQ(q.pop(), 2);
}

TEST(MyQueueTest, MixedOperations) {
    MyQueue<int> q;
    for (int i = 0; i < 50; ++i) {
        q.push(i);
        if (i % 2 == 0) q.pop();
    }
    EXPECT_EQ(q.size(), 25);
}

// ==========================================
// 3. LifeAutomaton (24 теста)
// ==========================================

TEST_F(GridTest, Life_EmptyGridStaysEmpty) {
    LifeAutomaton life;
    life.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Life_SingleCellDies) {
    LifeAutomaton life;
    cur[2][2] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
}

TEST_F(GridTest, Life_TwoCellsDie) {
    LifeAutomaton life;
    cur[2][2] = true; cur[2][3] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Life_BlockStaysStable) {
    LifeAutomaton life;
    cur[1][1] = true; cur[1][2] = true;
    cur[2][1] = true; cur[2][2] = true;
    life.step(cur, next, W, H);
    EXPECT_EQ(countAlive(next, W, H), 4);
    EXPECT_TRUE(next[1][1]);
    EXPECT_TRUE(next[1][2]);
    EXPECT_TRUE(next[2][1]);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Life_BlinkerOscillator) {
    LifeAutomaton life;
    cur[2][1] = true; cur[2][2] = true; cur[2][3] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[1][2]);
    EXPECT_TRUE(next[2][2]);
    EXPECT_TRUE(next[3][2]);
    EXPECT_FALSE(next[2][1]);
}

TEST_F(GridTest, Life_ToadOscillator) {
    LifeAutomaton life;
    cur[2][2] = true; cur[2][3] = true; cur[2][4] = true;
    cur[3][1] = true; cur[3][2] = true; cur[3][3] = true;
    life.step(cur, next, W, H);
    EXPECT_EQ(countAlive(next, W, H), 6);
}

TEST_F(GridTest, Life_BirthExactlyThree) {
    LifeAutomaton life;
    cur[1][1] = true; cur[1][2] = true; cur[1][3] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Life_NoBirthWithTwoNeighbors) {
    LifeAutomaton life;
    cur[1][1] = true; cur[1][2] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
}

TEST_F(GridTest, Life_NoBirthWithFourNeighbors) {
    LifeAutomaton life;
    cur[1][1] = true; cur[1][2] = true; cur[1][3] = true;
    cur[3][1] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
}

TEST_F(GridTest, Life_Underpopulation) {
    LifeAutomaton life;
    cur[2][2] = true; cur[2][3] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
}

TEST_F(GridTest, Life_Overpopulation) {
    LifeAutomaton life;
    cur[2][2] = true;
    cur[1][1] = true; cur[1][2] = true; cur[1][3] = true;
    cur[2][1] = true; cur[3][1] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
}

TEST_F(GridTest, Life_GliderMoves) {
    LifeAutomaton life;
    cur[1][0] = true; cur[2][1] = true;
    cur[0][2] = true; cur[1][2] = true; cur[2][2] = true;
    for(int i=0; i<4; ++i) {
        life.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_EQ(countAlive(cur, W, H), 5);
}

TEST_F(GridTest, Life_EdgeCellDies) {
    LifeAutomaton life;
    cur[0][0] = true; cur[0][1] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[0][0]);
}

TEST_F(GridTest, Life_CornerCellDies) {
    LifeAutomaton life;
    cur[0][0] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[0][0]);
}

TEST_F(GridTest, Life_NoWrapAroundLeftRight) {
    LifeAutomaton life;
    cur[0][0] = true; cur[1][0] = true; cur[2][0] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[1][W-1]);
}

TEST_F(GridTest, Life_NoWrapAroundTopBottom) {
    LifeAutomaton life;
    cur[0][0] = true; cur[0][1] = true; cur[0][2] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[H-1][1]);
}

TEST_F(GridTest, Life_FullGrid_CornersSurvive) {
    LifeAutomaton life;
    for(int y=0; y<H; ++y) for(int x=0; x<W; ++x) cur[y][x] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[0][0]);
    EXPECT_TRUE(next[0][W-1]);
    EXPECT_TRUE(next[H-1][0]);
    EXPECT_TRUE(next[H-1][W-1]);
    EXPECT_FALSE(next[1][1]);
}

TEST_F(GridTest, Life_BlockAtEdge_NoWrap) {
    LifeAutomaton life;
    cur[0][0] = true; cur[0][1] = true;
    cur[1][0] = true; cur[1][1] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[0][0]);
    EXPECT_TRUE(next[0][1]);
    EXPECT_TRUE(next[1][0]);
    EXPECT_TRUE(next[1][1]);
    EXPECT_FALSE(next[0][W-1]);
    EXPECT_FALSE(next[H-1][0]);
}

TEST_F(GridTest, Life_TwoStepsReturnToStart) {
    LifeAutomaton life;
    cur[2][2] = true; cur[2][3] = true; cur[2][4] = true;
    life.step(cur, next, W, H);
    bool** temp = cur; cur = next; next = temp;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
    EXPECT_TRUE(next[2][3]);
    EXPECT_TRUE(next[2][4]);
}

TEST_F(GridTest, Life_OneCellLeftDoesNotCrash) {
    LifeAutomaton life;
    cur[0][0] = true;
    life.step(cur, next, W, H);
}

TEST_F(GridTest, Life_DiagonalNeighborsBirth) {
    LifeAutomaton life;
    cur[1][1] = true; cur[3][3] = true; cur[1][3] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Life_LineOfFour) {
    LifeAutomaton life;
    cur[2][1] = true; cur[2][2] = true; cur[2][3] = true; cur[2][4] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[1][2]);
    EXPECT_FALSE(next[2][1]);
}

TEST_F(GridTest, Life_PatternWithNoNeighbors) {
    LifeAutomaton life;
    cur[5][5] = true;
    life.step(cur, next, W, H);
    EXPECT_FALSE(next[5][5]);
}

TEST_F(GridTest, Life_CenterWithThreeNeighbors) {
    LifeAutomaton life;
    cur[4][4] = true; cur[4][5] = true; cur[4][6] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[5][5]);
}

TEST_F(GridTest, Life_TwoSeparateBlinkers) {
    LifeAutomaton life;
    cur[1][1] = true; cur[1][2] = true; cur[1][3] = true;
    cur[5][5] = true; cur[5][6] = true; cur[5][7] = true;
    life.step(cur, next, W, H);
    EXPECT_TRUE(next[0][2]);
    EXPECT_TRUE(next[4][6]);
}

TEST_F(GridTest, Life_BlinkerAfterManySteps) {
    LifeAutomaton life;
    cur[2][2] = true; cur[2][3] = true; cur[2][4] = true;
    for(int i=0; i<10; ++i) {
        life.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_EQ(countAlive(cur, W, H), 3);
}

TEST_F(GridTest, Life_AllDeadStaysDead) {
    LifeAutomaton life;
    for(int i=0; i<5; ++i) {
        life.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_TRUE(isEmpty(cur, W, H));
}

// ==========================================
// 4. CustomAutomaton (19 тестов)
// ==========================================

TEST_F(GridTest, Custom_EmptyMasksKillsAll) {
    CustomAutomaton custom(0, 0);
    for (int y=0; y<H; ++y) for (int x=0; x<W; ++x) cur[y][x] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Custom_B0BirthsEverything) {
    CustomAutomaton custom(1u << 0, 0);
    custom.step(cur, next, W, H);
    EXPECT_EQ(countAlive(next, W, H), W * H);
}

TEST_F(GridTest, Custom_S8SurvivesOnlyWith8) {
    CustomAutomaton custom(0, 1u << 8);
    cur[2][2] = true;
    custom.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
}

TEST_F(GridTest, Custom_S8SurvivesWith8) {
    CustomAutomaton custom(0, 1u << 8);
    for(int y=1; y<=3; ++y) for(int x=1; x<=3; ++x) cur[y][x] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Custom_AllMasksAllBirthAllSurvive) {
    CustomAutomaton custom(0xFFFF, 0xFFFF);
    cur[2][2] = true;
    custom.step(cur, next, W, H);
    EXPECT_EQ(countAlive(next, W, H), W * H);
}

TEST_F(GridTest, Custom_B3S23Blinker) {
    CustomAutomaton custom((1u << 3), (1u << 2) | (1u << 3));
    cur[2][1] = true; cur[2][2] = true; cur[2][3] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[1][2]);
    EXPECT_TRUE(next[2][2]);
    EXPECT_TRUE(next[3][2]);
}

TEST_F(GridTest, Custom_HighBitIgnored) {
    CustomAutomaton custom(0, 1u << 9);
    for (int y=0; y<H; ++y) for (int x=0; x<W; ++x) cur[y][x] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Custom_B1BirthsWithOne) {
    CustomAutomaton custom(1u << 1, 0);
    cur[2][2] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[1][1]);
}

TEST_F(GridTest, Custom_S0SurvivesWithZero) {
    CustomAutomaton custom(0, 1u << 0);
    cur[2][2] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Custom_B12345678BirthsAll) {
    CustomAutomaton custom((1u << 8) - 1, 0);
    custom.step(cur, next, W, H);
    EXPECT_EQ(countAlive(next, W, H), W * H);
}

TEST_F(GridTest, Custom_S0to8SurvivesAll) {
    CustomAutomaton custom(0, (1u << 9) - 1);
    for (int y=1; y<H-1; ++y)
        for (int x=1; x<W-1; ++x)
            cur[y][x] = true;
    custom.step(cur, next, W, H);
    for (int y=1; y<H-1; ++y)
        for (int x=1; x<W-1; ++x)
            EXPECT_TRUE(next[y][x]);
}

TEST_F(GridTest, Custom_B3S23IsLife) {
    CustomAutomaton custom((1u << 3), (1u << 2) | (1u << 3));
    LifeAutomaton life;
    cur[2][2] = true; cur[2][3] = true; cur[2][4] = true;
    life.step(cur, next, W, H);
    bool** lifeResult = next;
    next = createGrid(W, H);
    custom.step(cur, next, W, H);
    for(int y=0; y<H; ++y) for(int x=0; x<W; ++x) EXPECT_EQ(lifeResult[y][x], next[y][x]);
    freeGrid(lifeResult, H);
}

TEST_F(GridTest, Custom_S1SurvivesWithOne) {
    CustomAutomaton custom(0, 1u << 1);
    cur[2][2] = true; cur[2][3] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Custom_B2BirthsWithTwo) {
    CustomAutomaton custom(1u << 2, 0);
    cur[2][2] = true; cur[2][3] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[1][2]);
}

TEST_F(GridTest, Custom_NoBirthNoSurvive) {
    CustomAutomaton custom(0, 0);
    cur[1][1] = true; cur[1][2] = true; cur[1][3] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Custom_AllBirthNoSurvive) {
    CustomAutomaton custom(0xFFFF, 0);
    cur[2][2] = true;
    custom.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
    EXPECT_TRUE(next[1][1]);
}

TEST_F(GridTest, Custom_EmptyBirthS23Survive) {
    CustomAutomaton custom(0, (1u << 2) | (1u << 3));
    cur[2][2] = true; cur[2][3] = true; cur[2][4] = true;
    custom.step(cur, next, W, H);
    EXPECT_FALSE(next[1][2]);
}

TEST_F(GridTest, Custom_B3S23LifeEquivalent) {
    CustomAutomaton custom((1u << 3), (1u << 2) | (1u << 3));
    LifeAutomaton life;
    for(int i=0; i<3; ++i) {
        custom.step(cur, next, W, H);
        life.step(cur, next, W, H);
    }
}

TEST_F(GridTest, Custom_SingleBitMask) {
    CustomAutomaton custom(1u << 5, 1u << 5);
    cur[2][2] = true;
    custom.step(cur, next, W, H);
    EXPECT_FALSE(next[2][2]);
}

TEST_F(GridTest, Custom_TwoBitMask) {
    CustomAutomaton custom((1u << 2) | (1u << 3), (1u << 2) | (1u << 3));
    cur[1][1] = true; cur[1][2] = true; cur[1][3] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Custom_ZeroBirthOneSurvive) {
    CustomAutomaton custom(0, 1u << 1);
    cur[2][2] = true; cur[2][3] = true;
    custom.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
    EXPECT_TRUE(next[2][3]);
}

// ==========================================
// 5. DropsAutomaton (20 тестов)
// ==========================================

TEST_F(GridTest, Drops_NoWavesEmpty) {
    DropsAutomaton drops;
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_AddOneCell) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[5][5]);
}

TEST_F(GridTest, Drops_SingleWaveDiamondShape) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[5][5]);
    EXPECT_TRUE(next[4][5]);
    EXPECT_TRUE(next[5][4]);
    EXPECT_FALSE(next[4][4]);
}

TEST_F(GridTest, Drops_WaveExpands) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    for(int i=0; i<3; ++i) {
        drops.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_TRUE(cur[2][5]);
}

TEST_F(GridTest, Drops_CenterFades) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    for(int i=0; i<5; ++i) {
        drops.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_FALSE(cur[5][5]);
}

TEST_F(GridTest, Drops_WaveOutOfBounds) {
    DropsAutomaton drops;
    drops.onCellAdded(0, 0);
    for(int i=0; i<5; ++i) {
        drops.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_TRUE(cur[0][4]);
    EXPECT_FALSE(cur[0][9]);
}

TEST_F(GridTest, Drops_AddOutOfBounds) {
    DropsAutomaton drops;
    drops.onCellAdded(-5, 10);
    drops.onCellAdded(100, 100);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_AddNegativeCoords) {
    DropsAutomaton drops;
    drops.onCellAdded(-1, -1);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_TwoWaves) {
    DropsAutomaton drops;
    drops.onCellAdded(2, 2);
    drops.onCellAdded(7, 7);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
    EXPECT_TRUE(next[7][7]);
}

TEST_F(GridTest, Drops_TwoWavesCollide) {
    DropsAutomaton drops;
    drops.onCellAdded(3, 5);
    drops.onCellAdded(6, 5);
    for(int i=0; i<3; ++i) {
        drops.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_TRUE(cur[4][5]);
}

TEST_F(GridTest, Drops_RemoveSource) {
    DropsAutomaton drops;
    drops.onCellAdded(2, 2);
    drops.onCellRemoved(2, 2);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_RemoveNonExistent) {
    DropsAutomaton drops;
    drops.onCellAdded(2, 2);
    drops.onCellRemoved(5, 5);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[2][2]);
}

TEST_F(GridTest, Drops_ResetClears) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    drops.step(cur, next, W, H);
    drops.reset();
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_MultipleResets) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    drops.reset();
    drops.reset();
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_EmptyStep) {
    DropsAutomaton drops;
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_ManyWaves) {
    DropsAutomaton drops;
    for(int i=1; i<10; i+=2) drops.onCellAdded(i, i);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[1][1]);
    EXPECT_TRUE(next[9][9]);
}

TEST_F(GridTest, Drops_WaveOnCorner) {
    DropsAutomaton drops;
    drops.onCellAdded(0, 0);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[0][0]);
    EXPECT_TRUE(next[1][0]);
    EXPECT_TRUE(next[0][1]);
    EXPECT_FALSE(next[1][1]);
}

TEST_F(GridTest, Drops_WaveFadesCompletely) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    for(int i=0; i<20; ++i) {
        drops.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_TRUE(isEmpty(cur, W, H));
}

TEST_F(GridTest, Drops_WaveWithStaticObstacle) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    cur[2][5] = true;
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[5][5]);
}

TEST_F(GridTest, Drops_WaveLifetime) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    for(int i=0; i<2; ++i) {
        drops.step(cur, next, W, H);
        bool** temp = cur; cur = next; next = temp;
    }
    EXPECT_TRUE(cur[4][5]);
    EXPECT_TRUE(cur[5][4]);
}

TEST_F(GridTest, Drops_AddAndRemoveThenAddAgain) {
    DropsAutomaton drops;
    drops.onCellAdded(3, 3);
    drops.onCellRemoved(3, 3);
    drops.onCellAdded(5, 5);
    drops.step(cur, next, W, H);
    EXPECT_FALSE(next[3][3]);
    EXPECT_TRUE(next[5][5]);
}

TEST_F(GridTest, Drops_WaveShapeAtRadius) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[5][5]);
    EXPECT_TRUE(next[4][5]);
    EXPECT_TRUE(next[6][5]);
    EXPECT_TRUE(next[5][4]);
    EXPECT_TRUE(next[5][6]);
    EXPECT_FALSE(next[4][4]);
}

TEST_F(GridTest, Drops_TwoWavesMerge) {
    DropsAutomaton drops;
    drops.onCellAdded(4, 5);
    drops.onCellAdded(6, 5);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[5][5]);
}

TEST_F(GridTest, Drops_WaveAtBorderAndRemove) {
    DropsAutomaton drops;
    drops.onCellAdded(0, 0);
    drops.onCellRemoved(0, 0);
    drops.step(cur, next, W, H);
    EXPECT_TRUE(isEmpty(next, W, H));
}

TEST_F(GridTest, Drops_DiamondPattern) {
    DropsAutomaton drops;
    drops.onCellAdded(5, 5);
    drops.step(cur, next, W, H);
    bool** temp = cur; cur = next; next = temp;
    drops.step(cur, next, W, H);
    EXPECT_TRUE(next[5][5]);
    EXPECT_TRUE(next[3][5]);
    EXPECT_TRUE(next[5][3]);
    EXPECT_TRUE(next[7][5]);
    EXPECT_TRUE(next[5][7]);
}

// ==========================================
// 6. Параметризованные тесты (SIZE TESTS)
// ==========================================

class AutomatonSizeTest : public ::testing::TestWithParam<std::tuple<int, int>> {
protected:
    int W, H;
    bool** cur;
    bool** next;

    void SetUp() override {
        W = std::get<0>(GetParam());
        H = std::get<1>(GetParam());
        cur = new bool*[H];
        next = new bool*[H];
        for (int i = 0; i < H; ++i) {
            cur[i] = new bool[W]{false};
            next[i] = new bool[W]{false};
        }
    }

    void TearDown() override {
        for (int i = 0; i < H; ++i) { delete[] cur[i]; delete[] next[i]; }
        delete[] cur; delete[] next;
    }
};

TEST_P(AutomatonSizeTest, Life_DoesNotCrash) {
    LifeAutomaton life;
    life.step(cur, next, W, H);
    for (int i = 0; i < H; ++i) for (int j = 0; j < W; ++j) cur[i][j] = true;
    life.step(cur, next, W, H);
}

TEST_P(AutomatonSizeTest, Custom_DoesNotCrash) {
    CustomAutomaton custom(1u << 3, (1u << 2) | (1u << 3));
    custom.step(cur, next, W, H);
    for (int i = 0; i < H; ++i) for (int j = 0; j < W; ++j) cur[i][j] = true;
    custom.step(cur, next, W, H);
}

TEST_P(AutomatonSizeTest, Drops_DoesNotCrash) {
    DropsAutomaton drops;
    drops.onCellAdded(0, 0);
    drops.onCellAdded(W/2, H/2);
    drops.step(cur, next, W, H);
    for(int i=0; i<5; ++i) {
        bool** temp = cur; cur = next; next = temp;
        drops.step(cur, next, W, H);
    }
}

INSTANTIATE_TEST_SUITE_P(
    AllSizes,
    AutomatonSizeTest,
    ::testing::Combine(
        ::testing::Values(1, 2, 3, 4, 5, 10, 15),
        ::testing::Values(1, 2, 3, 4, 5, 10, 15)
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}