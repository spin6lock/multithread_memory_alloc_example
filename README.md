说明
========
可以自定义线程对的数量(PAIR_SIZE)，线程对之间用pipe通信，producer分配内存，consumer释放内存(内存大小用SIZE控制)。测试发现jemalloc的峰值内存会比glibc的高，且达到峰值后不再下降。可以用MALLOC_CONF环境变量设置，`MALLOC_CONF="dirty_decay_ms:0,muzzy_decay_ms:0"`可以极大降低内存峰值，`MALLOC_CONF="background_thread:true"`也有效果，起码内存会降下来。没有这两个设置，内存一直停留在130M左右不再下降
