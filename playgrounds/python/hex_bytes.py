
from typing import List
from xrpl.utils import str_to_hex

def hex_to_byte_array_for_c(hexstr: str):
  array = bytearray.fromhex(hexstr)
  print(len(array))
  list = { "U, ".join(hex(b).upper() for b in bytes(array)) }
  print(list)


# ----------------------------------------------------------------------------------------->
# BUILD URI

# hexstr: str = str_to_hex('ipfs://QmaCtDKZFVvvfufvbdy4estZbhQH7DXh16CTpv1howmBGy')
# hex_to_byte_array_for_c(hexstr)

# 697066733a2f2f516d614374444b5a4656767666756676626479346573745a626851483744586831364354707631686f776d424779
# {'0X69U, 0X70U, 0X66U, 0X73U, 0X3AU, 0X2FU, 0X2FU, 0X51U, 0X6DU, 0X61U, 0X43U, 0X74U, 0X44U, 0X4BU, 0X5AU, 0X46U, 0X56U, 0X76U, 0X76U, 0X66U, 0X75U, 0X66U, 0X76U, 0X62U, 0X64U, 0X79U, 0X34U, 0X65U, 0X73U, 0X74U, 0X5AU, 0X62U, 0X68U, 0X51U, 0X48U, 0X37U, 0X44U, 0X58U, 0X68U, 0X31U, 0X36U, 0X43U, 0X54U, 0X70U, 0X76U, 0X31U, 0X68U, 0X6FU, 0X77U, 0X6DU, 0X42U, 0X47U, 0X79'}

# hexstr: str = str_to_hex('00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002')
# hex_to_byte_array_for_c(hexstr)

# ----------------------------------------------------------------------------------------->
# BUILD NFTOKENID

hexttr = '00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002'
list = { "U, ".join(f'0x0{b}' for b in hexttr) }
print(list)

# 00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002
# {'0X30U, 0X30U, 0X30U, 0X39U, 0X30U, 0X30U, 0X30U, 0X30U, 0X45U, 0X33U, 0X45U, 0X32U, 0X36U, 0X34U, 0X39U, 0X46U, 0X42U, 0X38U, 0X34U, 0X44U, 0X46U, 0X42U, 0X30U, 0X35U, 0X35U, 0X30U, 0X33U, 0X36U, 0X36U, 0X30U, 0X35U, 0X42U, 0X34U, 0X39U, 0X44U, 0X45U, 0X33U, 0X30U, 0X35U, 0X36U, 0X31U, 0X38U, 0X42U, 0X41U, 0X37U, 0X32U, 0X39U, 0X37U, 0X32U, 0X44U, 0X43U, 0X42U, 0X41U, 0X42U, 0X39U, 0X44U, 0X30U, 0X30U, 0X30U, 0X30U, 0X30U, 0X30U, 0X30U, 0X32'}
