# Common-Data-Structures

*A collection of classic data structures — linked lists, a generic dynamic array, a hash set, and a hash-based dictionary — built from scratch in C.*

![C](https://img.shields.io/badge/language-C-00599C.svg?logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

---

## 🔎 overview/

- **What it is:** hand-rolled implementations of core data structures in pure C — a singly linked list, a doubly linked list, a generic dynamic array, a hash-based set, and a hash-based dictionary. No STL-equivalent, no external libraries, no safety net.
- **Why:** built to actually internalize how these structures behave under the hood — memory layout, pointer arithmetic, and the workarounds C forces on you when the language hands you no generics and no bounds checking.
- **Context:** solo project, built in about a week as a focused low-level exercise rather than a production library.
- **Target / intended use:** a learning artifact and portfolio reference — for anyone curious how common structures look when implemented directly in C, without a language runtime doing the heavy lifting.
- **What I learned:**
  - Working with low-level, manually-laid-out data structures instead of relying on a language's built-in containers.
  - Reasoning explicitly about algorithmic efficiency — traversal cost, chaining/rehashing strategy, `memmove`-based shifting — instead of trusting a library implementation.
  - Approximating language features C doesn't have natively (generics, polymorphism) through deliberate workarounds: byte-size-driven generics for the array, tagged unions for the dictionary.
  - Building and consuming a dynamic library (`.dll`): the linked list compiles as a shared library that the set and dictionary link against.
- **Current status:** *supported* — each structure is functionally complete for the operations it exposes, but the project is deliberately left open: there's an effectively unlimited list of other structures and algorithms that could be added on top of this base.

## 🕹️ functional/

- **Tech stack:** C, no external dependencies — standard library only (`stdio.h`, `stdlib.h`, `string.h`, `stdint.h`, `time.h`).
- **Requirements:** any C compiler. The linked list module is built and exported as a Windows DLL (`__declspec(dllexport)` guarded by `_WIN32`), so the reference build targets MSVC/MinGW on Windows.
- **Layout:**
  - `include/` — public headers per structure (`linked_list.h`, `doubly_linked_list.h`, `dynarray.h`, `set.h`, `dictionary.h`).
  - `src/<structure>/` — implementation + `_tests.c` file per module.
  - `lib/linked_list/` — the compiled `linked_list.dll` / `.lib` / `.exp`, consumed dynamically by the set and dictionary modules.
- **Setup:** compile each `src/<structure>/<structure>.c` against its header; the `linked_list` module builds as a shared library first, since `set` and `dictionary` link against it for their bucket storage.
- **How it's used:** each module ships its own `_tests.c` main that exercises the API directly and prints results to the console — there's no external test framework. For example, `dynarray_tests.c` initializes an array sized for a custom struct, reserves capacity in bulk, appends items in a loop, and prints live allocation stats (`malloc_counter`, total bytes allocated) after every operation.

- **Configuration:** the dynamic array's element size is caller-driven — `dynarray.size_of_element` must be set (typically via `sizeof(your_type)`) before `dynarray_init`. The set and dictionary both start with an initial slot count via `hash_map_setup` and grow automatically from there.
- **Demo/build:** no prebuilt binary or playable demo — this is a source library, verified through its test executables.

## ⚙️ technical/

**Architecture.**
Every structure follows the same split: a public header in `include/` and an implementation file in `src/<name>/`. Two different strategies stand in for C's lack of generics, chosen per structure's actual needs:

- **Dynamic array — byte-level genericity.** `dynarray_t` stores a raw `void* data` plus an explicit `size_of_element`. Since pointer arithmetic on `void*` isn't legal C, every offset calculation casts through `uint8_t*` first (`DYNARRAY_UINT8_DATA + SIZEOF(index)`), then uses `memmove`/`memcpy` to shift or copy elements. This makes the array genuinely type-agnostic — it can hold anything from a `uint16_t` to a custom struct — at the cost of losing compile-time type safety entirely.
- **Set / Dictionary — tagged genericity.** Rather than trying to make the hash table itself generic, the dictionary tags each value with a `type_t` enum (`INT`, `STRING`) and dispatches on it inside `print_value`/`print_item`. It's closer to a hand-rolled tagged union than true generics, but it was the more honest fit: the dictionary needs to *format* its values, not just store opaque bytes.

**Key algorithms and data structures:**

- **Linked lists (singly & doubly).** Both use an *intrusive* node pattern: `struct list_node` (or its doubly-linked variant with `prev`/`next`) is embedded as the first member of `string_item`, so a `string_item*` can be freely cast to/from a `list_node*`. This is the same trick that lets the set and dictionary reuse the linked list's traversal code for their hash buckets.
- **Hash set / dictionary.** Separate chaining, with each bucket backed by a linked list. Hashing uses a djb33x variant (`hash = (hash << 5) + hash) ^ key[i]`, seeded at 5381 — djb2 with XOR instead of addition for the character mix). Rehashing (doubling the slot count and re-distributing every key) triggers as soon as *any* bucket's chain length exceeds 1 — a deliberately aggressive policy that trades more frequent table doublings for keeping buckets almost always at length ≤ 1, prioritizing lookup speed over allocation churn.
- **Dynamic array allocation tracking.** `aiv_malloc` / `aiv_realloc` / `aiv_free` wrap the standard allocator and update two global counters (`malloc_counter`, `malloc_total_allocations`) — a small, self-built instrumentation layer instead of reaching for an external profiler, used directly in the test output to sanity-check allocation behavior.
- **Doubly linked list shuffle.** Implemented as repeated random extraction: on each pass, pick a random remaining node, unlink it, and append it to a new list — an O(n²) approach (it walks the list to find each random index) rather than an in-place Fisher–Yates, favored here for reusing the existing remove/append primitives instead of writing index-based swaps.

**Hardest decision.** Deciding how to fake genericity without C++ templates, and realizing partway through that one technique doesn't fit every structure — the array needed to store *any* POD type by raw size, while the dictionary needed to know enough about its value's type to print and dispatch on it. Using two different approaches instead of forcing one generic pattern everywhere was the actual design call.

**A deliberate shortcut worth flagging.** `remove_string_item` (and its dictionary equivalent, `hash_map_remove`) take a `remove_item` flag and double as *lookup* functions when the flag is `0` — `get_item` and `get_list_item` are thin wrappers that call the removal function with the flag off. This avoids writing a second traversal for lookups, at the cost of a function whose name doesn't fully describe what it does when you're not paying attention to that flag.

**Testing.** No formal test suite or benchmark harness — each module's `_tests.c` main drives the API directly against a handful of scenarios and verifies behavior by reading console output (insert/remove sequences, allocation counters, rehash logs), rather than asserting programmatically.
