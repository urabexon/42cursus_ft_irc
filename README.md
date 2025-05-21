# 42cursus_ft_irc📶

この課題は、42Tokyo本科Common Core(基礎課程)のLevel5に位置する課題です。<br>
C++を用いてIRCプロトコルに準拠したサーバーを一から実装することで、ソケットプログラミング、ノンブロッキングI/O、多重接続処理、IRCコマンドの仕様理解などを深め、軽量なIRCサーバーを構築します。<br>
IRCクライアントとの通信確認のために `netcat` や `irssi` を用いた検証も行っています。

## Usage💻

このプログラムの使用方法は以下の通りです。

1. **コンパイル方法**
   - リポジトリのルートディレクトリで `make` を実行してください。
   - 実行ファイル（例: `ircserv`）が生成されます。

2. **実行方法**
   - 以下の形式で起動します。
     ```bash
     ./ircserv <port> <password>
     ```
   - 例：
     ```bash
     ./ircserv 6667 secretpass
     ```

3. **動作確認方法**
   - `nc` (netcat) で簡易的な接続テストが可能です。
     ```bash
     nc localhost 6667
     ```
   - その後、以下のようなIRC初期コマンドを入力します。
     ```
     PASS secretpass
     NICK john
     USER john 0 * :jack john
     ```

## Implementation Function🎓

このプロジェクトで実装した主な機能は以下の通りです。

- IRCプロトコルの基本的なコマンドの処理
  - PASS / NICK / USER / JOIN / PART / PRIVMSG / QUIT などに対応
- クライアント管理
  - クライアントの認証、ユーザー状態、チャンネル参加状況を管理
- 複数クライアントとの通信処理
  - `poll()` によるノンブロッキングI/Oで多数同時接続に対応
- チャンネル機能
  - チャンネル作成、メンバー管理、メッセージのブロードキャスト処理
- モード機能（OP権限、INVITE制限、KEYパスワードなど）
- Invite制、kick、topic設定などのチャンネル操作コマンド
- サーバーからの適切な数値レスポンス (`RPL_`, `ERR_`) を返却

## Working Point💡

- RFC 1459 に基づいたIRCコマンド仕様の読み込みと再現
- pollによる多重接続処理と各クライアントの状態管理
- クラスごとの責務分離（Client / Server / Channel 等）
- チャンネルごとの制御（invite-only、トピック設定など）
- シグナル（Ctrl+C）を受け取った際の優雅なサーバー終了処理

## Points Learned📋

- ソケットプログラミングと `poll()` によるノンブロッキングI/Oの基礎
- IRCプロトコル（特にクライアントの状態遷移）の理解
- 状態管理とイベント駆動型の設計（非同期処理の考え方）
- コマンド単位の責務分割とクラス構成の設計力
- サーバープロセスとしてのログ管理

## Directory Structure🌲
全体のディレクトリ構成です。

```
├── .gitignore
├── Makefile
├── README.md
├── design_document_ftirc.pdf
├── includes
    ├── Channel.hpp
    ├── Client.hpp
    ├── Command.hpp
    ├── Irc.hpp
    ├── IrcNumerics.hpp
    └── Server.hpp
└── srcs
    ├── Channel.cpp
    ├── Client.cpp
    ├── Server.cpp
    ├── commands
        ├── commandUtils.cpp
        ├── invite.cpp
        ├── join.cpp
        ├── kick.cpp
        ├── mode.cpp
        ├── modes
        │   ├── invite.cpp
        │   ├── key.cpp
        │   ├── limit.cpp
        │   ├── operator.cpp
        │   └── topic.cpp
        ├── nick.cpp
        ├── part.cpp
        ├── pass.cpp
        ├── privmsg.cpp
        ├── quit.cpp
        ├── topic.cpp
        └── user.cpp
    ├── handlePollFd.cpp
    ├── main.cpp
    ├── manageServerLoop.cpp
    ├── parse.cpp
    └── utils.cpp
```

## Development Period📅
- 2025/04/22~2025/05/21 (30日間 / 約1ヶ月)

## Reference🔖
- [RFC 1459 - Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [IRSSI IRCクライアント](https://irssi.org/)