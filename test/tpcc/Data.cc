// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "Data.h"
//---------------------------------------------------------------------------
namespace imlab_test {
//---------------------------------------------------------------------------
const char* const kTestWarehouse = R"RAWCSV(4|LPESIfo|aiAcXSImDqLf|wVqSpx1e3DNVdD3mo|2vIomPtgo1U3PJUgHYr|bw|584811111|.1973|3000000.00
1|2Xtbfe|XlZDbdUdAn6B9wq5qmzV|8SGj3S1diu|qFlC5kZz3Rk|Nb|657611111|.1923|3000000.00
5|f616uFN|sEZcx4foeppu|NZgdLndt5qAE4btW9H|0hoEsJKQTC7XoM|oG|334311111|.1938|3000000.00
2|hbgihp9pfk|8Uheuf5hvj3YjQHXNVLr|xPUx0Y5ICDEr|dClA1jnPNt|vL|470911111|.1040|3000000.00
3|z0Y98H|SLtA4eJWLXusdTB|4kmgyhsiT6xKR505F|YFmTwj83737pFjSX|jy|299411111|.1157|3000000.00
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestDistrict = R"RAWCSV(1|4|5u3yvni|QregMnUUiBguHR7IvCQ|YGr4aw648K|s6Omh1ILJqaO6|ll|517111111|.0168|3000000.00|3001
2|4|aoWdUqPR|rYJvhEktAJeOgY2jTS|H8vvscHXHp|zD5C8hhb5g1FHNfv8R|Kl|989611111|.0291|3000000.00|3001
3|4|4dOPHsw|KuybiNFndQ|ZPlr3EOnjN5VOlbQt2H|Hrsi4ymO3tx8SaoB9uaz|yz|178711111|.1447|3000000.00|3001
4|4|wW9weTe3y6|Qdn1NPPS7tTuYanmK0Tv|3ZMeed1wf70uUmguGL5|MvDc5JCHO9wh|tE|236111111|.0645|3000000.00|3001
5|4|SZMyVsLt|Px9J1MA137oZUpR|MUV6ByPie3tzSaHyYZ|rzXSe5K8YE18utjmba|I6|347711111|.1694|3000000.00|3001
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestHistory = R"RAWCSV(1|1|1|4|4|39|10.00|L1xLeaMBVGZwT50
2|1|1|4|4|39|10.00|KGOgpAuKu5qZnug
3|1|1|4|4|39|10.00|537REaEs8cxFbtETX3
4|1|1|4|4|39|10.00|bhHoZ8hM9KGBjzZ0l9
5|1|1|4|4|39|10.00|BsyT67Fv6L3xI
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestItem = R"RAWCSV(1|3164|hIHbFrcahBDz0ej7|67.80|p6reTzhitjOHtkvrzBfPo7qX2Mh7WvrL0Rbp
2|2426|Yz8my7wFQHwAKYCEk|42.98|EIw2Ra3ZcgzkGjpS2X5SM6qEHoPvyfN6wWM4muaH6u
3|7932|oWQqVJnrm8VDZHMt|57.71|ZGf0dfyXQNeZhIpm2WNx6Gku0PRnRDuGzFCGytuIYM
4|6316|8qUlMje8oxzZk3L6hr9TDaG|95.84|VF7A9HA3FNezzW2EYnHXUNQ1wcsNE5H4ra4x2DpZ
5|3030|4rFdyTRNZzb1ASi|11.53|TSmQoNBYtzhzjM1zBwVSfPzgjtgonp8xfiTaecnk651D
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestNewOrder = R"RAWCSV(2101|1|4
2102|1|4
2103|1|4
2104|1|4
2105|1|4
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestOrder = R"RAWCSV(1|1|4|2946|39|9|10|1
2|1|4|1404|39|7|13|1
3|1|4|1730|39|6|6|1
4|1|4|1557|39|4|10|1
5|1|4|1335|39|1|6|1
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestOrderLine = R"RAWCSV(1|1|4|1|36861|4|39|5|0.00|eZ4Gm8NmGHTbalXoY3bO2RLo
1|1|4|2|36444|4|39|5|0.00|KjUPAh6wEvJr6M4ruFKjUWML
1|1|4|3|82684|4|39|5|0.00|1QNGPaciKPKuF1YJFBmtjdTO
1|1|4|4|1174|4|39|5|0.00|8weGQGsyWLG5mCcroktusLJS
1|1|4|5|58521|4|39|5|0.00|0oMjl1FjItO84lS30q12Mr4c
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestStock =
    R"RAWCSV(1|4|50|E2ziJ56SXnbFcrcnDi5FKqRv|AwPAys0aNNHP4o9SlHpkJI2t|FNVIJAjFFANrHERWNCjnhszI|h7NrfFS9UufVWRtaUwEElAsA|gMhJEauViF4FpJU4UcUpvnpj|AICeLd3JVQ1buyyB4oWBlFB3|5J7pCvPxMOCvd2a14rWQGNyZ|kVaZZn0baRTfc9NyanHcX48f|Of21nORYZTecK1KrPhYz3wEX|3cG81VzizwSI04WrbYtbofns|0|0|0|q81z6IbDzyHZDrc1LxG0ze0h9Vmk60EW9JR75lMr9cVj
2|4|57|mzpinD70eGOMDksyzExf5sdi|dlgObIQycGqFBTVQLiDMwmYF|nJU5H3nzknmrsL8BNuy2bhHY|3OsXJwiDbbepHdU0NU8ZNWXq|4fWq2moWkRc0ASjXVbQUeDAI|2Zi2j3vLWaXjZ4LpZ0YrHh97|MYMHmeUvP12B8LsCVmLbVthq|QaK8mz69Dsrkxhq2hO2wKAs4|ixBvJJoIiu0Wp9mcRN80ne3B|HqztpECTLVlqCrkUEcUYNOG5|0|0|0|Rlwl2jbTUXBtdU6NUjKOB41aHwbf8gL7qsFfzOL4X
3|4|67|XMix1R0RXileaNkhuWAt4SOZ|VxmebO0N0paQAgU9fbpva3jk|2dM06LrPLAPstgxKYb7mbhoO|tHJltXDURELwfT43zj6x8as7|yxjwNuxPaOXvG2AR0nfPUNin|6198gwjHgKENtaFCWeUxkfy2|KDAS3ONcS9O4iINZoztXnOpV|SXtMfCvDEG44JxQz9lQc7VyH|31ZbKIV0UumYKpG3x1QBYupe|1hTZzsF813EQjlqDjkmedH1Z|0|0|0|tlDIw8cJY19gAN3odrxih8Gm9oAc
4|4|71|Am8R8tYM6oKCxriB6CkSezZo|fDy1uBk5ZaYtceO5FbzPL5bq|EboymCmEM9zme71VXg8weuA6|ACD5ODlYxZsJrObIYOKl1UoM|Gcqbk1hzD18bggUNvpzcKCl1|Jw5FW94blPmPj0ptpc2JzIQt|zwjxUxzpvpCxWZrYULTZsCeM|IFNApJq0u2hbKzQgiGwqQWDm|g7qxo4ugFcVOcXkGU6RnP6Gm|btR7DgZ4aDMFvswxKju0Ck78|0|0|0|1FoJloIOIgGihDIKwFVNkRlopuSE329cIjaSTvYlt7VXe
5|4|63|RZE43Zz3GkswnimMJHLU4r75|CEVw1OMGjjF32e7YbYqjxWQe|BLbrfMI0svYH2WWFeU6RlW8u|OoR0PeRTCJOlGTlqi3Z64b6j|edF4OWTIBwMLFutu8ePwDlfD|ADR4GXFGREGn09QsfSfRT0e5|J5QmAlSQ3JZf25glhc5dOBqx|2SvoMb2r5ddlDlCL7jR1YYpb|EtjIGVgstcFq7m4ihUqQBKNq|aZlE8BtyWvLJcSHbZ1RGJuhM|0|0|0|Ny90MORIGINAL
)RAWCSV";
//---------------------------------------------------------------------------
const char* const kTestCustomer = R"RAWCSV(1|1|4|XvTC9cQ1BnpH3ue|OE|BARBARBAR|ik1yiPkonwJ|pAxmX3VomS5QzAP1K|SPtt5EcqNaafy|hD|525811111|9613662508411977|39|GC|50000.00|.4211|-10.00|1.00|0|0|OsfGdm8UJFMPvwSA3lASWL8Aq941eu2K9bB406Rzp3Z2L3W29631NhfBugH98eeF5JxGNJimKMtwhM0o2F8lKgEw3dPTOSj1cN1w6gTUUk585rG2QIEo0PGO5QZifJWePMXK7GvXrnFrrjeNgbO89eaQiYI2Z7HiSa7RI6bEOGpBxmZ2OHfPsZrzk023YIYksYQfkwAcWOS2vneHL3aoCRliI0HD7gVSt5QnMKOfKSW3GZdUFnK4IU4YFOsAz9G3GuzFWJlshTTGtHXcm8AfrDJKoHtU6jaj5tX8bcUob2O15SX1ayRbVMpp0GwZSZ48KvbAwL37xt9CLZL4mWNc0vMSg3WI9xpG7PVQ3XOb3nTyV7tZ
2|1|4|tZhugfr7h|OE|BARBAROUGHT|TnMb0jwcz8rD|4zIePdXwuBxB2MTz1J|PBG2lSGTSRjeESt|YW|853311111|1380558293894392|39|GC|50000.00|.4309|-10.00|1.00|0|0|OUDz2Oks7MOcq8AbjImJHodwu6ySQhlqrkOCOwc5SrEkKlbrKpsVWYTMJEgrf61CCz6pdmJWsiE2Tjwj1fBlRJue1nr4yyBadR6ZRE16nqlybhlmwDxlKhoN6aC7yFw3YocMHYF9bOZtK6Euqq6ufXw03VfrsIKe2SXis2IArgMxUxIFQoT2qRixo42SxsKFZWBrQywqVpyfrjSGfIEP4fhgYX4ustGMr1pYjeK9yllsdBSV3vLiXAQr466d4DSucKSKtvh1VGzx6B1XRIb0dvBrtw8vqETb357yQIGSDNtlmos12wmtGzKyiQ4c2Ry8K1NsRCs8dcfpR6rderItsXbpTHFCZdPc9nV0ndVnY3Rq8W9DLFiFZmSHXS9iCRuotBlivGb9
3|1|4|5Y3pDQPluD|OE|BARBARABLE|pGUAhwUSkZjlQDhft|Z6lb9NHPlwQB40fL|Ah8Mt701qXDjg1M|nR|011811111|5460462831141653|39|GC|50000.00|.0568|-10.00|1.00|0|0|ds88L42UfgQ2AMVsyWwKmpvwiYGH5YFu5CCiT3tQwCJTZc4fEJu95gipRQ41BMe59cN7a4q0h9qrjyU5gS4CO54LAr1SHMhkGNqGi0MYJudMGkgvpyoX7tw0hF0dR49F98HMMUkoOzjnUaUFS3CB668kqmTVMtNiceX1CUz4hgfjKaSdfOOk0bkcbBEW2d2MuPP0Me9P7cX3SvarZUCIXYYWqWmhpz0XIj5iGMGp5QtanIUERpCwGk5DkVCPtUhBSSXeFYhM8MAvX5sgNDUADRA3zpLu8dCEnDgM4e5mhYNiJm2CB3jMA3eRglAhhUbPMAQoFVQl7xgG3
4|1|4|m5RtgNHgFVLm5s|OE|BARBARPRI|2otIyzgS9Ujacme7c|eZVAgJQQO9EPFoLZO7l5|5ZjwXCddMKT|LP|185911111|8995658202909469|39|BC|50000.00|.0627|-10.00|1.00|0|0|xB9MU4SYMZ4o3GdJPmGueTLHfOs6PWvTTy22Ut3RvMhCHHzk9F7eFZx8ZXm40LVCq1aJgCmx0UyVU9acsfA9uUIZ4zDMCIkwem8WFP3eEztzT4bs8C4fDMvv4qIIcbGzkeRtg7lLabSOLc9fwQ7rUShIFrkmeaxjOzo9AXZYlh44oYO5SWZDbMyfSn3zlDu81FTObT9ZqjA7QblLygSvbFHNOqY6dsutozknwDg1ovyzdHqqTdINg4eiKsKzm5p9sh4VhJT6BsPLCRLnl02r2enqm6RWLLba8uT4rkIXoGKs5JxL38Ce9hft8oh8eEmGMnudEVF65kGTxbiDRyg8ZY5BIF9YKRJWQl9REmfTRrhIDXGu7PRoi5ViDCuNfgHBYGXylMEUnYA0hbA3wVSHQqJXyWIt4SlsZQmfpuNPoRZLMp5lH4BJAerQJ13uJwMFVPXDeXbk27I2pxHNzEkxwPeiZwUxfThzw
5|1|4|hLZIr005mWLKQ|OE|BARBARPRES|KDA2fS8ZglqXqzvZ|DJZzfm2B49K|i6XGnPhlVnx7dD|a4|067511111|1730971003131127|39|GC|50000.00|.3095|-10.00|1.00|0|0|7wX0mtI3BdkLxLzk2ehlVtBjGwQXMdfLfw2QjUMG3e5ykilKYIU0yYLklpMvQXfBquChWErPCh1cpEhcdTJgpZC7DUTcOHsMQG6w72zWZ7FFt484UikQR0jFTKDr0cbwPJrN6m1uAh78D77i1T8lW3JWc1xbW1CyQ3rYx6mRsBeOXSpFcizU9N9gWlTWyrIhyt46oEIGPLHf2CpvODQPAbp5aU78bnVWO8rQNRJkvwRqmcN8SgE7ivzJMaZFVOuvDnh6Unqg55lQtKSvtnnhpIHMtubn6LHpgIzoeLBShVtMDsxo33jlzuASHJ6oR51Qgqm5gLuPJ6kTYcoR33JRiy51skE
)RAWCSV";
//---------------------------------------------------------------------------
} // namespace imlab_test
//---------------------------------------------------------------------------