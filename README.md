# tabcat

Convert data in a csv format into a neat table.

## Usage

```shell
$ cat table.csv
numbers;a;words
1;b;thing
40;c;sea
66;d;car
1024;e;ultralongword
$ cat table.csv | tabcat
┌─────────┬───┬───────────────┐
│ numbers │ a │ words         │
├─────────┼───┼───────────────┤
│       1 │ b │ thing         │
│      40 │ c │ sea           │
│      66 │ d │ car           │
│    1024 │ e │ ultralongword │
└─────────┴───┴───────────────┘
```

