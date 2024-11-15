#server.py
import asyncio
import websockets

# Define the WebSocket server behavior
async def hello(websocket):
    name = await websocket.recv()  # Receive a message from the client
    print(f'Server received: {name}')
    
    greeting = f'Hi {name}'  # Create a greeting
    await websocket.send(greeting)  # Send the greeting back to the client
    print(f'Server sent: {greeting}')

# Start the WebSocket server
async def main():
    # Specify the host and port, and launch the WebSocket server
    async with websockets.serve(hello, "localhost", 8765):
        await asyncio.Future()  # Keep the server running

# Run the WebSocket server if this script is run as the main program
if __name__ == "__main__":
    asyncio.run(main())