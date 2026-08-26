# VisualResourceLinkerForSiv3D

Siv3D のリソース埋め込みを補助するツール。`Resource.rc` を GUI で編集する。

## 使い方

File > Open (Ctrl+O) で `Resource.rc` を開く。読み込んだリソースは Required / Optional / Custom の
3 つのタブに分かれて並ぶ。

各行のチェックボックスで有効と無効を切り替える。無効にした行は `//` でコメントアウトされる。
Erase ボタンを押せば行ごと消える。ただし Required のリソースは Siv3D 本体が要求するので消せない。

追加は Regist ボタンか、ウィンドウへのドラッグ＆ドロップ。落としたファイルは Custom タブに入り、
パスはプロジェクトフォルダからの相対パスへ自動で直る。

書き戻しは File > Save (Ctrl+S)。File > Revert (Ctrl+R) を選ぶと、最後に読み込んだ時点まで
変更が全部巻き戻る。

## バックアップ

ファイルを開いた時点で自動的にバックアップを取る。置き場所は元のファイルと同じフォルダ。

```
Resource.rc.2025-08-21_13-35-00.bak
```

戻すときは手作業になる。いまの `Resource.rc` を消すか別名にして退避し、戻したい `.bak` の名前を
`Resource.rc` に変える。それだけ。
