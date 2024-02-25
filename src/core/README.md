# core

このツールが管理するタスクデータ構造を置くための階層階層

## クラス


```plantuml

class Project
class Task
class WorkItem

together {
class Theme
class Category
}
class Section

Project -do- Theme

Theme -do- Task
Task -do- WorkItem

Task --> Category

note right of Project
プロジェクトを表すクラス
end note

note right of Theme
プロジェクト内の作業テーマを表すもの
要件単位だったり、一つの障害だったり、どういう単位でくくるかは人による
IssueTracker系ツールの親チケットに相当する?
end note

note right of Task
Themeに紐づく実際の作業を表す
IssueTracker系ツールの子チケットみたいな?
end note

note right of WorkItem
Taskに紐づいた作業の作業記録を表す単位
作業を中断したりした場合はその分のWorkItemインスタンスが存在する形になる
作業時間の情報はここで持つ
end note

note bottom of Category
作業の分類を表すもの
設計、実装、テスト、打合せ、雑用・・
end note

note right of Section
時間区分を表すクラス
end note


```

