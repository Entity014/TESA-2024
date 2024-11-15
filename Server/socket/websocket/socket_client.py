# client.py
import asyncio
import websockets

async def send_message():
    uri = "ws://localhost:8765"
    try:
        async with websockets.connect(uri) as websocket:
            # Define the message to send
            message = input("messege :")
            
            # Send the message to the server
            print(f"Sending message to server: {message}")
            await websocket.send(message)
            
            # Wait for the server's response
            response = await websocket.recv()
            print(f"Received response from server: {response}")
    except websockets.ConnectionClosed as e:
        print(f"Connection closed with code {e.code} and reason: {e.reason}")

# Run the client
asyncio.run(send_message())