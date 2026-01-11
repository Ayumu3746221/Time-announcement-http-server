# Time Announcement Server

A simple TCP server that returns the current time to connected clients.

## Overview

This is a learning project for network programming in C. When a client connects to port 8080, the server responds with the current timestamp and closes the connection.

### Features

- IPv4/IPv6 dual-stack support
- Single-connection handling
- Returns current time in `YYYY-MM-DD HH:MM:SS` format

## Usage

```bash
# Build
gcc server.c -o server

# Run
./server

# Test (in another terminal)
nc localhost 8080
```

## Reference

This project was built following [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).

---

# 時刻通知サーバー

接続したクライアントに現在時刻を返すシンプルなTCPサーバーです。

## 概要

C言語でのネットワークプログラミング学習用プロジェクトです。クライアントがポート8080に接続すると、サーバーは現在時刻を返して接続を閉じます。

### 機能

- IPv4/IPv6 両対応
- シングルコネクション処理
- `YYYY-MM-DD HH:MM:SS` 形式で現在時刻を返却

## 使い方

```bash
# ビルド
gcc server.c -o server

# 実行
./server

# テスト（別ターミナルで）
nc localhost 8080
```

## 参考

このプロジェクトは [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) を参考に作成しました。
