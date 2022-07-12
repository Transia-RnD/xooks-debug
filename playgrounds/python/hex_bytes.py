
from typing import List
from xrpl.utils import str_to_hex

def hex_to_byte_array_for_c(hexstr: str):
  array = bytearray.fromhex(hexstr)
  print(len(array))
  list = { "U, ".join(hex(b).upper() for b in bytes(array)) }
  print(list)


# ----------------------------------------------------------------------------------------->
# BUILD URI

hexstr: str = str_to_hex('ipfs://bafkreif2whi7g3wexrrqylr75pdwfimrpy45i4mhkbu3bvvlw3rm66ge7e?filename=nic-nft-1.jpg')
hex_to_byte_array_for_c(hexstr)

# 697066733a2f2f516d614374444b5a4656767666756676626479346573745a626851483744586831364354707631686f776d424779
# {'0X69U, 0X70U, 0X66U, 0X73U, 0X3AU, 0X2FU, 0X2FU, 0X51U, 0X6DU, 0X61U, 0X43U, 0X74U, 0X44U, 0X4BU, 0X5AU, 0X46U, 0X56U, 0X76U, 0X76U, 0X66U, 0X75U, 0X66U, 0X76U, 0X62U, 0X64U, 0X79U, 0X34U, 0X65U, 0X73U, 0X74U, 0X5AU, 0X62U, 0X68U, 0X51U, 0X48U, 0X37U, 0X44U, 0X58U, 0X68U, 0X31U, 0X36U, 0X43U, 0X54U, 0X70U, 0X76U, 0X31U, 0X68U, 0X6FU, 0X77U, 0X6DU, 0X42U, 0X47U, 0X79'}

# hexstr: str = str_to_hex('00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002')
# hex_to_byte_array_for_c(hexstr)

# ----------------------------------------------------------------------------------------->
# BUILD NFTOKENID

# hexttr = '00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002'
# array = bytearray.fromhex(hexttr)
# list = { "U, ".join(hex(b).upper() for b in bytes(array)) }
# print(list)

# 00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002
# {'0X0U, 0X9U, 0X0U, 0X0U, 0XE3U, 0XE2U, 0X64U, 0X9FU, 0XB8U, 0X4DU, 0XFBU, 0X5U, 0X50U, 0X36U, 0X60U, 0X5BU, 0X49U, 0XDEU, 0X30U, 0X56U, 0X18U, 0XBAU, 0X72U, 0X97U, 0X2DU, 0XCBU, 0XABU, 0X9DU, 0X0U, 0X0U, 0X0U, 0X2'}
