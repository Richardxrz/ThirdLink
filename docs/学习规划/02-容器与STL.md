# 知识库 02：容器与 STL

> 归属阶段：阶段 0 / 阶段 3。range-for、array/vector/span 详解、容器使用频率排行与选型口诀。

## 容器的分类地图（先看这张图）

    序列容器（按位置组织）：vector / array / deque / list / forward_list
        元素按「第几个」排列，关心位置（第一个/最后一个/中间）
    关联容器（按键组织，自动排序）：map / set / multimap / multiset
        元素按「键」排列，关心「按什么找到」，遍历时自动有序
    无序关联容器（按键，哈希，不排序）：unordered_map / unordered_set / ...
        元素按键组织，哈希表实现，查找 O(1)，不保证顺序
    适配器（收窄接口）：stack / queue / priority_queue
        底层复用其他容器，只暴露受限操作
    视图（不拥有数据）：span / string_view

## 迭代器（你已经在用，只是不知道名字）

迭代器 = 指向容器中某个元素的「指针的通用化」。range-for 的底层就是迭代器：

    for (auto x : v) { ... }
    等价于：
    for (auto it = v.begin(); it != v.end(); ++it) {
      auto x = *it;
      ...
    }

- begin()：指向第一个元素；
- end()：指向「最后一个元素之后」的哨兵位置（不是最后一个元素！）；
- *it：解引用，取出 it 指向的元素；
- ++it：移动到下一个元素；
- it != end()：判断是否遍历完。

迭代器让「算法与容器解耦」：std::sort(begin, end)、std::find(begin, end, x)
只需要迭代器，不需要知道容器是 vector 还是 list——这是 STL 的核心设计。

## 适配器与「收窄接口」

适配器 = 建立在其他容器之上、只暴露受限操作的容器：

    std::stack<int> s;          // 底层默认 deque，但只给你 push/pop/top
    s.push(1); s.pop(); s.top();  // 不能用 s[0]，不能遍历

「收窄」= 把底层容器的接口砍掉一大半，只留语义需要的操作。
「接口」= 一个类型对外暴露的操作集合（你能对它做什么）。
为什么收窄：防止误用（栈本就不该随机访问）、语义清晰（看到 stack 就知道后进先出）、
底层可换（stack 的底层可以是 deque/vector/list，模板参数决定，不影响使用方）。

### 栈的两层含义（数据结构的栈 vs 运行时的调用栈）

「栈」这个词在 C++ 里有三层，别混：

1. 抽象概念：LIFO 数据结构（push/pop/top）——逻辑规则，不关心内存；
2. std::stack：C++ 标准库的适配器——用代码实现这个逻辑（底层是 deque/vector，堆上分配）；
3. 调用栈（call stack）：程序运行时真实存在的内存区域——每个函数调用压一个「栈帧」
   （局部变量、参数、返回地址），函数返回时弹出。

为什么函数调用天然就是栈：f 调用 g，g 必须最先返回、f 才能继续——
嵌套调用的顺序天生就是后进先出。递归每层压一个栈帧，递归过深 = 栈溢出（stack overflow）。

所以：调用栈 = 「栈」这个数据结构在程序运行时最经典的应用，不是巧合，是必然。

### 为什么会有 FIFO？怎么实现的？

FIFO（先进先出）对应现实世界的「排队」语义：先来先服务。
需要它的场景：打印机任务队列、消息队列（先发的先处理）、BFS 广度优先搜索
（先发现的节点先扩展）、按提交顺序处理请求。

LIFO 适合「撤销/回溯/嵌套」（栈），FIFO 适合「排队/按序处理」（队列）——两个不同语义需求。

FIFO 的实现 = 一端进、另一端出，关键是**两端都要 O(1)**：

    std::queue 底层默认 deque：push 进队尾、pop 出队头，两端都 O(1)
    为什么不用 vector？vector 头部操作是 O(n)（要搬动所有元素）

嵌入式（没有堆/想固定大小）常用**环形缓冲区**：固定数组 + 头尾两个下标，
取模循环（rear = (rear+1) % size），O(1) 且零动态分配——你回嵌入式时会亲手写它。

range-for 是遍历「范围」（数组、vector、array、span、字符串、初始化列表）每个元素的简洁语法，自动处理边界。

for (const Segment& s : segs) 拆解：
- Segment：元素类型；
- const：只读，不修改元素；
- &：引用，直接引用容器里那个元素，而不是拷贝一份；
- s：循环变量名；
- :：读作「遍历……中的每个」；
- segs：被遍历的容器/范围。

等价于传统写法：
    for (size_t i = 0; i < segs.size(); ++i) {
        const Segment& s = segs[i];
        ...
    }

常见变体：
    for (Segment s : segs)          // 拷贝（小对象没问题，大对象浪费）
    for (const Segment& s : segs)   // 只读引用，最常用
    for (Segment& s : segs)         // 可修改元素
    for (const auto& s : segs)      // auto 自动推导类型

## 1.8 容器：std::array / std::vector / std::span

std::array<T, N>：定长数组（C 数组的直系升级）。
    #include <array>
    std::array<Segment, 3> segs{};   // 零初始化
    segs.size();                      // 3，编译期常量
    auto copy = segs;                 // 整体复制，不用 memcpy
- 内存布局和 C 数组完全一样（栈上、连续、零开销），但不退化为指针；
- 支持 = 复制、== 比较、.at(i) 边界检查；
- 用在哪：数量编译期已知且永不改变。

### array 的「坑位哲学」：先有空间，后填值（回应：没数据存进去没关系）

「数量编译期已知」只决定一件事：创建时就在栈上内嵌 N 个元素的【坑位】。
和 C 数组一样，**空间立即存在，值可以之后慢慢填**：

    std::array<int, 3> a{};    // 坑位在 + 值全部零初始化（内建类型 = 0）
    std::array<int, 3> b;      // 坑位在，但内建类型的值【不确定】（未初始化，读取是未定义行为）
    a[0] = 42;                 // 之后随时填值，完全没问题

三种状态对照 C 数组（行为完全一致）：

    int a[3];              // 空间在，值不确定
    int a[3] = {1, 2, 3};  // 空间在，值给了
    int a[3] = {};         // 空间在，值全部零

注意：如果结构体成员带默认初始化器（你 Link 的 origin{}/length{}/angle{}），
那么即使不带 {} 创建，元素也会被默认成员初始化器安全归零——你的 Link 数组怎么写都不会有未初始化风险。

为什么 N 必须是编译期常量：array 的空间内嵌在对象里（栈上），编译器必须知道开多大；
vector 的空间在堆上、运行时动态，所以 N 才可以是运行时的。

std::vector<T>：动态数组（堆上，自动管理）。
    #include <vector>
    std::vector<Segment> segs;
    segs.reserve(3);                        // 预分配容量
    segs.push_back(Segment{...});           // 追加
    segs.emplace_back(Segment{...});        // 就地构造，少一次拷贝
    segs.size(); segs.capacity();           // 元素数 / 已分配容量
    segs[1].phi = 0.5f;                     // 无检查访问（快）
    segs.at(1).phi = 0.5f;                  // 有边界检查，越界抛异常
- 自己管理内存（RAII）：自动扩容、析构自动释放，没有 malloc/free 配对；
- 内存连续，缓存友好；
- 用在哪：数量运行时才知道或会变化 → 这就是「连杆数不写死」的需求。

### emplace_back 的「原地构造」是怎么实现的（回应：vector 怎么知道放哪）

vector 内部只有三个东西：起始地址 data、有效元素数 size、已分配容量 capacity。
有效元素永远是 [0, size) 连续一段；[size, capacity) 是「已分配但未构造」的原始内存。

    capacity = 5
    ┌─────┬─────┬─────┬─────┬─────┐
    │ e0  │ e1  │ e2  │ ??? │ ??? │   ← 已分配内存
    └─────┴─────┴─────┴─────┴─────┘
    size = 3          ↑
               data + size（下一个构造位置）

emplace_back 的流程（伪代码）：
    if (size == capacity) 扩容();          // 没空位 → 分配更大的内存，搬移旧元素
    new (data + size) T(args...);          // placement new：在末尾位置直接构造，不分配新内存
    ++size;

所以「原地」= 在 vector 已分配的内存里直接构造，而不是先造临时对象再拷贝进来。
对比 push_back：
    v.push_back(Link{100.0f, 0.5f});   // 1. 外部构造临时 Link  2. 拷贝/移动进 v
    v.emplace_back(100.0f, 0.5f);      // 直接拿参数在 v 末尾位置构造（一步，省一次拷贝）

关键：vector 从不「找空位」——语义要求元素连续无洞，新元素永远追加在末尾（size 位置）；
「空位」（capacity-size）只是预留的原始内存，不是洞。没空位时走扩容：分配更大内存、
把旧元素搬过去、释放旧内存，然后继续在末尾构造。

std::span<T>（C++20）：非拥有视图（指针+长度 二合一）。
    #include <span>
    void DrawArm(std::span<const Segment> segs) {
        for (const Segment& s : segs) { ... }
    }
    std::array<Segment,3> a; std::vector<Segment> v; Segment raw[3];
    DrawArm(a); DrawArm(v); DrawArm(raw);   // 三种都能传
- 内部就是指针+长度，零开销，不分配、不拥有内存；
- 是 C 惯用法「T* ptr + int count」的现代标准化版本；
- 用在哪：函数参数想接收任意一段连续序列（DrawArm 就是完美用例）。

### span 的数量从哪来？（为什么调用时不用传数量）

span 有两种构造方式：

1. 从容器隐式构造（调用方不用传数量）：
    std::vector<Link> v;
    std::span<const Link> s = v;   // 自动取 v.data()（指针）+ v.size()（数量）
    所以 DrawArm(arm) 直接调用即可——vector 自动转换成 span，指针和数量都自动带上。

2. 从裸指针显式构造（必须传数量）：
    Link* data = ...;              // 裸指针不携带长度信息
    std::span<Link> s(data, 3);    // 指针 + 数量（3 个元素）

「数量」= 元素个数（数组里有几个 Link），不是成员数量、不是字节数。

对照表：
    T[N] / std::array   拥有   编译期定   栈     数量固定不变
    std::vector         拥有   运行时定   堆     数量可变/可配置
    std::span           不拥有 运行时定   视图   函数参数

对 Arm 的建议：成员用 std::vector<Segment>（拥有数据），DrawArm 收 std::span<const Segment>（不关心来源）。

## 1.9 关于 span 理解的确认

你的理解基本正确，补一处修正：
- 对：span 主要不是用来「定义拥有数据的变量」，而是规定形参的使用方式；
- 对：它封装了「指针 + 长度」；
- 修正：它不是「和指针调用方式一致」那么简单——它知道长度，能 range-for、.size()、.subspan() 切片段，
  这些是裸指针没有的。而且它能从 array/vector/C 数组隐式构造，所以调用方直接传容器即可，不必手动给指针。

### span 的 const 是什么意思（std::span<const Link> 的 const 修饰谁）

const 修饰的是【元素】Link，不是 span 对象本身：

    std::span<const Link> s;   // 元素只读：s[0].length = 50; 编译错误
                               // span 本身可重绑：s = 别的span; 合法
    const std::span<Link> s;   // 元素可改：s[0].length = 50; 合法
                               // span 不可重绑：s = 别的span; 编译错误
    const std::span<const Link> s;  // 元素只读 + 不可重绑

所以 DrawArm 用 std::span<const Link> 的语义是：「我只读这些数据，不会改它们」——
绘制不应该修改机械臂状态，这个 const 就是把这个意图写进类型里（const 正确性）。

顺带：vector<Link> 能隐式转 span<const Link>（可读视图），
但不能转 span<Link>（不能把可变视图接给只读数据）。

关键澄清：**span 本身不是只读的**——std::span<T> 完全可写（可以改元素），
只读来自元素类型上的 const（std::span<const T>），是写代码的人主动加的 const，不是 span 自带。

「不拥有」≠「只读」，是两个独立维度：
    std::span<T>          不拥有数据，但可以修改元素（视图指向别人的数据，能改）
    std::span<const T>    不拥有数据，也不能修改元素

和指针完全对应：
    std::span<T>      ≈ T*        （可变视图）
    std::span<const T> ≈ const T* （只读视图）

为什么 API 里常见 span<const T>：因为 span 的典型用法是「只读接收一段序列」，
惯例是主动写 const T，享受 const 正确性检查——这是惯例，不是 span 的特性。

---


---

## 频率总览（经验值）
    1. std::vector        🔥🔥🔥 默认选择：运行时才知道数量的动态集合
    2. std::string        🔥🔥🔥 一切文本：路径、日志、解析、格式化
    3. std::unordered_map 🔥🔥   按键查找：字典/缓存/计数/配置表
    4. std::map           🔥🔥   需要按键有序遍历、范围查询
    5. std::array         🔥    编译期固定大小：查表/常量集合
    6. std::span/string_view 🔥 现代 C++ 函数参数：零拷贝只读视图
    7. std::queue/stack   🔥    BFS / DFS / 撤销栈 / 表达式求值
    8. std::priority_queue 🟡   Top-K / Dijkstra·A* / 按优先级调度
    9. std::deque         🟡    双端 O(1) 增删：滑动窗口、双端缓存
    10. std::list         🟢    中间频繁增删 + 迭代器稳定 + splice（新手慎用）
    11. multimap/multiset 系列 🟢 一key多值（多数可用 map<K, vector<V>> 替代）

80/20 法则：前 4 名覆盖 95% 需求。

## 决策口诀
    动态集合     → vector        （默认！）
    按键查值     → unordered_map （要排序就 map）
    固定大小     → array
    只读参数     → span / string_view
    先进先出     → queue
    后进先出     → stack
    每次取最值   → priority_queue
    头尾都增删   → deque
    中间增删+稳定迭代器 → list（最后才考虑）

## 关键警示
- list 是新手最容易误用的：现代 CPU 缓存友好性使 vector 通常更快，list 只在「中间插入 + 迭代器稳定性」同时需要时才选；
- unordered_map 查找 O(1)，map 查找 O(log n) 但按键有序——只要查找不要排序就选 unordered_map；
- span/string_view 不拥有数据，注意生命周期（被引用对象销毁后继续用 = 悬垂）；
- 序列容器共同方法：size/empty/begin/end/clear/insert/erase/swap/==；
- 关联容器特有：m[key]、at(key)、find、count、contains(C++20)、erase(key)；有序版额外 lower_bound/upper_bound；
- 适配器刻意收窄接口：stack(push/pop/top)、queue(push/pop/front/back)、priority_queue(push/pop/top)。

---

