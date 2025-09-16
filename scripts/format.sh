find . -iname '*.cc' -o -iname '*.h' -o -iname '*.h.in' | xargs clang-format -i --style=file
