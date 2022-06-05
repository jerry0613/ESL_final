# 資料夾
- final_basic: version basic的合成結果
- final_pipe: version pipelining的合成結果
- final_parallel: version parallelism的合成結果
- final_parallel2: version parallelism2的合成結果
- final_opt: version Opt的合成結果
- riscv-vp: RISCV-VP的軟體和platform
# 執行
進到riscv-vp/sw/final_sw後，使用`make origin`可以執行version basic的模擬，使用`make pipe`可以執行version pipelining的模擬，使用`make paraller`可以執行version parallelism的模擬，執行結果會在`output_vertex.txt`中。