# tabcat

Convert data in CSV format into a neat table.

## Usage


Suppose you have data in CSV format, such as this:

```
numbers;a;words
1;b;thing
40;c;sea
66;d;car
1024;e;ultralongword
```

Using `tabcat` you can arrange it in a nice table.
The program expects the data on standard input (`stdin`), so to pass the data to `tabcat`, you can use these commands for example:

```shell
$ cat data.csv
numbers;a;words
1;b;thing
40;c;sea
66;d;car
1024;e;ultralongword

$ tabcat data.csv
┌─────────┬───┬───────────────┐
│ numbers │ a │ words         │
├─────────┼───┼───────────────┤
│       1 │ b │ thing         │
│      40 │ c │ sea           │
│      66 │ d │ car           │
│    1024 │ e │ ultralongword │
└─────────┴───┴───────────────┘
```

