# Wutong Publisher · 梧桐出版

[![CI](https://github.com/project-wutong/wutong-publisher/actions/workflows/ci.yml/badge.svg)](https://github.com/project-wutong/wutong-publisher/actions/workflows/ci.yml)
Qt 6 GUI (C++23) + Rust 2024 engine · `AGPL-3.0-only`

## 中文

完整的本地多格式编译/构建与导出（Typst、LaTeX、Pandoc → PDF/EPUB/HTML）、文献引用检查。联合出版投射（`wutong.publication`）与撤回为可选增强。

**离线优先**：无需账户、网络、Server、后台或 AI 即可完成本地核心功能（OFFLINE-001）。

### 架构（Rust 优先）

```
src/ (C++ / Qt Widgets, 仅 GUI)
   │  cxx bridge (crates/wutong-publisher-bridge)
   ▼
crates/wutong-publisher-core (纯 Rust 领域逻辑，可独立测试)
   ▼
wutong-core (协议核心：解析、规范化、签名、策略)
```

### 构建

```bash
cargo test --workspace                       # Rust 引擎与桥接
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build   # 需要 Qt ≥ 6.4（目标基线 6.12 LTS）
./build/wutong-publisher
```

## English

Wutong Publisher — the GUI layer is C++/Qt; everything else (document model, protocol, storage) is Rust,
reached through a `cxx` bridge. Works fully offline.
