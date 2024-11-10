# CustomAttackRange

自定义物品攻击距离

由于实现的并不完美且无法优化，只能准星模式下自定义攻击距离

​感谢由 子沐 提供的 Hook 点位

## 安装 / Install

```bash
lip install github.com/engsr6982/CustomAttackRange
```

## 使用 / Usage

- plugins/CustomAttackRange/config/Config.json

```json
{
  "version": 1,
  "items": {
    "minecraft:stick": 50.0 // key: Item type name, value: Attack range
  }
}
```
