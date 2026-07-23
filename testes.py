import socket
import struct

# 1. Cria o socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 6871))

print("Escutando calibração do RoboCup na porta 6871...")

while True:
    data, addr = sock.recvfrom(1024)
    
    # Verifica se recebemos o pacote exato de 21 bytes (4 int + 16 floats + 1 bool)
    if len(data) == 21:
        # '<I4f?' desempacota:
        # < = Little-endian (padrão de PC)
        # I = uint32_t (ID da mensagem)
        # 4f = 4 floats (Os sliders)
        # ? = bool (O botão power)
        pacote = struct.unpack('<I4f?', data)
        
        msg_id = pacote[0]
        sliders = pacote[1:5]
        power = pacote[5]
        
        print(f"ID: {msg_id:04d} | Sliders: {sliders[0]:6.1f}, {sliders[1]:6.1f}, {sliders[2]:6.1f}, {sliders[3]:6.1f} | Power: {power}")