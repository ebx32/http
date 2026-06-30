# Notes on Strings and ATS (All That Stuff)

## Pointers

A pointer is a variable which stores the address of another variable.

```c
int num = 5; 
int *num_ptr; // num_ptr is a pointer to an int.
```

```
    num                   num_ptr
+---------+            +---------+
|    5    |            | 0x1000  |
+---------+            +---------+
   0x1000                0x2000
```

We can dereference a pointer by using the dereferencing `*` operator.

```c
printf("%p\n", num_ptr);    // 0x1000
printf("%p\n", &num_ptr);   // 0x2000
printf("%d\n", *num_ptr);   // 5
```

## Strings & String Decay

A **string** is just an array of characters. And arrays store data in contiguous memory locations.

```c
char header[] = "200 OK HTTP/1.1";
```

Would give us something like this:

```
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+----+
| 2 | 0 | 0 |   | O | K |   | H | T | T | P | / | 1 | . | 1 | \0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+----+
```

`\0` is a **null terminator** which indicates the end of a string. **String docmposition** treats an array name as a pointer to its first position.

```
header == &header[0]
True
```

When we print this string with the string format specifier `%s`, `printf` reads from `&header[0]` till it reaches a null terminator then stops.

```c
printf("%s\n", header);     // 200 OK HTTP/1.1
printf("%s\n", &header[0])  // 200 OK HTTP/1.1
```

Since `header` is a pointer to the first memory location of the string, we don't need to use the reference operator `&` to assign a pointer it's address, since that's wrong. Instead we do:

```c
char *header_ptr = header;
// same as
char *header_ptr = &header[0];
```

Now `header` and `header_ptr` points to the same location.

> [!note]
> **What does that make `&header`**
> It points to the entire array. Imagine `header = 0x1000`. Then `header + 1 = 0x1001`, but `&header + 1 = 0x1000 + 16 = 0x1016`. 

## `strep()`

I used this a lot in `http.c`. It's mostly for parsing the request. 

1. Store the HTTP request into `req._buffer`: `req._buffer` = `request_buffer`
2. create a pointer to `req._buffer`: `char *parse_buffer = req._buffer`
3. Pass it's address `strsep()` with a custom delimiter: `strsep(&parse_buffer, " ")` 

`strsep()` takes a double pointer `**stringp` and a delimiter `" "`, parses the string by advancing the pointer, till it reaches the delimiter, which it replaces it with a null terminator `\0` and returns the address of the start of the isolated string.

It's a bit risky since it modifies the string in place. Meaning if we tried printing the string using the variable it's stored in, we end up printing the characters up to `\0` and not the entire string.

```c
char fruits[] = "apple banana watermelon"
char *parse_buffer = fruits;

char *apple = strsep(&parse_buffer, " ");
char *banana = strsep(&parse_buffer, " ");
char *watermelon = parse_buffer;

printf("%s\n", req._buffer); // apple

//final string: "apple\0banana\0watermelon"
```
