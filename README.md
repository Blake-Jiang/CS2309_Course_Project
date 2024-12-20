# 24点挑战游戏 (24 Challenge)

这是一个基于经典24点游戏的交互式应用程序。玩家需要使用四个给定的数字，通过基本运算得到24。

## 编译
在ucrt中执行

```
fltk-config --compile main.cpp game_window.cpp solve24.cpp 
```

即可生成可执行文件main.exe。

然后执行

```
./main
```

即可进入游戏界面


## 文件输入检查

点击File按钮，会自动读取test.txt文件中的内容，并根据要求生成结果，结果会显示在test_result.txt文件中。
## 游戏规则

1. **基本规则**
   - 每轮游戏会随机生成4个数字（1-13，对应扑克牌A-K）
   - 使用这4个数字和基本运算符（+, -, *, /）计算出24
   - 每个数字必须且只能使用一次
   - 可以使用括号改变运算顺序

2. **计时规则**
   - 可选择30秒/60秒/120秒的游戏时间
   - 时间耗尽前需要完成计算
   - 最后10秒计时器会变红提醒
   - 时间结束会显示一个可能的正确答案

3. **计分规则**
   - 基础分：100分
   - 时间奖励：
     * 剩余≥45秒：1.5倍
     * 剩余≥30秒：1.2倍
     * 剩余≥15秒：1.0倍
     * 剩余<15秒：0.8倍
   - 首次答对奖励：+50分
   - 连击奖励：每次连续答对+10分
   - 答错惩罚：-50分

## 操作说明

1. **开始游戏**
   - 选择游戏时间（右上角下拉框）
   - 点击"Start"开始新一轮

2. **输入答案**
   - 在答案输入框中输入计算表达式
   - 格式示例：(3+5)*(7-2)
   - 点击"Check"验证答案

3. **其他功能**
   - "Reset"：重置游戏状态
   - "File"：处理批量测试文件

## 答案格式说明

- 使用数字和运算符：+, -, *, /
- 可以使用括号：(1+2)*(3+4)
- 数字之间必须用运算符分隔
- 答案必须使用所有给定的数字
- 计算结果必须精确等于24

## 游戏技巧

1. 尽快答题以获得更高的时间倍数
2. 保持正确率以维持连击奖励
3. 注意观察时间，合理分配思考时间
4. 熟练使用括号改变运算优先级
5. 记住常见的数字组合

## 注意事项

- 答错会扣除50分，但不会出现负分
- 时间耗尽后需要重新开始新一轮
- 游戏会自动保存最高分记录

