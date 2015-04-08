the original readFile() will let 
a line ```455	24.2 ...``` keep the whitespace after the data


Also,  a line like 
``` ... 87749.32 90008.195 89234.125 94023.703 81841.187 80407.328 87277.234 83661.562 88899.094 116159.977 117132.156 208820.781 ...   ```
 will keep the whitespace before and after the data


In all, there are always 32 colomns, 

so I can add the whitespaces after the data, which means ```455	24.2 ...``` is fine,

but how can I add whitespaces before the data? 

``` ... 87749.32 90008.195 89234.125```

Before ```87749.32```