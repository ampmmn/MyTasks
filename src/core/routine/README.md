# core/routine

ルーチンタスク回りのクラス定義を行う

## ルーチンタスク

年/月/週/日ごとに繰り返し実施するタスク群を定義することができる

### 具体的な例

- 毎日
- 毎週月曜
- 毎週火曜
- 毎週水曜
- 毎週木曜
- 毎週金曜
- 毎週土曜
- 毎週日曜
- 週初稼働日
- 週最終稼働日
- 月初稼働日
- 月最終稼働日
- 年初稼働日
- 年最終稼働日

初稼働日/最終稼働日を正しく判断しようとすると、休業日を正しく定義しておく必要がある。   
休業日データは`core/Calenar`クラスで扱う

### クラス図

#### 全般

```plantuml

class RoutineFile

class RoutineRepository

class RoutineProviderIF
class RoutineIF

RoutineRepository -le-> RoutineFile : Load/Save

RoutineRepository "1" o.do. "*" RoutineProviderIF
RoutineRepository "1"  o.do. "*" RoutineIF

RoutineProviderIF ..> RoutineIF : 生成

note left of RoutineFile
ルーチン関係のデータをファイルから読み書きするためのクラス
end note

note right of RoutineRepository
ルーチン関係のデータを管理するクラス
シングルトンクラス
end note

note right of RoutineProviderIF
ルーチンクラス(RoutineIF)を生成するためのファクトリクラス
シングルトンクラス
end note

note right of RoutineIF
ルーチンを表すクラス
種類に応じて実装クラスが存在する
end note

```

#### プロバイダクラス

```plantuml

class RoutineProviderIF
class BaseRoutineProvider
class DailyRoutineProvider
class WeeklyRoutineProvider
class MonthlyRoutineProvider
class YearlyRoutineProvider

BaseRoutineProvider -up-|> RoutineProviderIF
DailyRoutineProvider -up-|> BaseRoutineProvider
WeeklyRoutineProvider -up-|> BaseRoutineProvider
MonthlyRoutineProvider -up-|> BaseRoutineProvider
YearlyRoutineProvider -up-|> BaseRoutineProvider

```

#### ルーチンクラス

```plantuml

class RoutineIF

class BaseRoutine

class DailyRoutine
class WeeklyRoutine
class MonthlyRoutine
class YearlyRoutine

BaseRoutine -up-|> RoutineIF

DailyRoutine -up-|> BaseRoutine
WeeklyRoutine -up-|> BaseRoutine
MonthlyRoutine -up-|> BaseRoutine
YearlyRoutine -up-|> BaseRoutine

note bottom of DailyRoutine
日次のルーチン
(曜日関係なく、毎日やるタスク)
end note

note bottom of WeeklyRoutine
週次ルーチン
例: 毎週最終稼働日に自席ごみ箱を空にする
月/火/水/木/金/土/日/週初稼働日/週最終稼働日をサポート
end note

note bottom of MonthlyRoutine
月次ルーチン
月初稼働日/月最終稼働日のみをサポート
end note

note bottom of YearlyRoutine
年次ルーチン
年初稼働日/年最終稼働日のみをサポート
end note

```

#### プロバイダとルーチンクラス間

```plantuml

together {
class DailyRoutineProvider
class DailyRoutine
}

together {
class WeeklyRoutineProvider
class WeeklyRoutine
}

together {
class MonthlyRoutineProvider
class MonthlyRoutine
}

together {
class YearlyRoutineProvider
class YearlyRoutine
}

DailyRoutineProvider -[hidden]do- WeeklyRoutineProvider
WeeklyRoutineProvider -[hidden]do- MonthlyRoutineProvider
MonthlyRoutineProvider -[hidden]do- YearlyRoutineProvider

DailyRoutineProvider -ri->DailyRoutine : create 
WeeklyRoutineProvider -ri->WeeklyRoutine : create 
MonthlyRoutineProvider -ri->MonthlyRoutine : create 
YearlyRoutineProvider -ri->YearlyRoutine : create 



```
