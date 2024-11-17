import React, { useEffect, useState } from "react";
import { LineChart, Line, CartesianGrid, XAxis, YAxis, Tooltip, Legend, Brush, ResponsiveContainer } from "recharts";
import { Button, Checkbox } from "@mui/material";

const App = () => {
  const [message, setMessage] = useState('');
  const [receivedMessage, setReceivedMessage] = useState(null);
  const ws = new WebSocket('ws://localhost:8081');
  ws.onopen = () => {
    console.log('WebSocket connection established');
  };

    ws.onmessage = (event) => {
    console.log('Received message from server:', event.data);
    setReceivedMessage(event.data); // Update state with received message
  };

  const handleSend = () => {
    const msg = 'Trigger to send {"hardware": "sent"} to MQTT';
    ws.send(msg);  // Send message to WebSocket server
    setMessage(msg); // Optionally display the sent message in App.js
  };
  
  
  const handleStart = () => {
    const msg = 'Trigger to send {"hardware": "true"} to MQTT';
    ws.send(msg);  // Send message to WebSocket server
    setMessage(msg); // Optionally display the sent message in App.js
  };
  
  const handleStop = () => {
    const msg = 'Trigger to send {"hardware": "false"} to MQTT';
    ws.send(msg);  // Send message to WebSocket server
    setMessage(msg); // Optionally display the sent message in App.js
  };

  const handleTest = () => {
    const msg = 'Trigger to send {"hardware": "sent2"} to MQTT';
    ws.send(msg);  // Send message to WebSocket server
    setMessage(msg); // Optionally display the sent message in App.js
  };


  const [data, setData] = useState([]);
  const [connectionStatus, setConnectionStatus] = useState("Connecting...");
  const [isPlotting, setIsPlotting] = useState(true);
  const [activePlots, setActivePlots] = useState({
    energy: true,
    pressure: true,
    force: true,
    position: true,
  });
  const [dataLogs, setDataLogs] = useState([]);
  const [selectedLogData, setSelectedLogData] = useState(null);
  const [selectedRange, setSelectedRange] = useState(null);

  useEffect(() => {
    const apiKey = "0d98d6c5c65ad999791d9a222d6b2ce7";
    const connection = new WebSocket("ws://technest.ddns.net:8001/ws");

    connection.onopen = () => {
      console.log("WebSocket connected");
      setConnectionStatus("Connected");
      connection.send(apiKey);
    };

    connection.onerror = (error) => {
      console.error("WebSocket Error:", error);
      setConnectionStatus("Error connecting to WebSocket.");
    };

    connection.onmessage = (event) => {
      if (!isPlotting) return;

      const message = event.data;
      if (message === "Connection authorized") return;

      try {
        const jsonData = JSON.parse(message);
        const newDataPoint = {
          timestamp: new Date().toLocaleString(),
          energy: jsonData["Energy Consumption"]?.Power,
          pressure: jsonData.Pressure,
          force: jsonData.Force,
          position: jsonData["Position of the Punch"],
        };

        setData((prevData) => {
          const updatedData = [...prevData, newDataPoint];

          if (updatedData.length > 200) {
            const logEntry = {
              id: Date.now(),
              range: `${updatedData[0].timestamp} - ${updatedData[199].timestamp}`,
              data: updatedData.slice(-200),
            };
            setDataLogs((prevLogs) => [...prevLogs, logEntry]);
            return updatedData.slice(-200);
          }
          return updatedData;
        });

      } catch (err) {
        console.error("Error parsing message:", err);
      }
    };

    connection.onclose = (event) => {
      console.log("WebSocket connection closed:", event.reason);
      setConnectionStatus("Disconnected");
    };

    return () => connection.close();
  }, [isPlotting]);

  const handleStartStop = () => setIsPlotting(!isPlotting);


  const handleCheckboxChange = (event) => {
    const { name, checked } = event.target;
    setActivePlots((prev) => ({ ...prev, [name]: checked }));
  };

  const handleLogClick = (logEntry) => {
    setSelectedLogData(logEntry.data);
    setSelectedRange(logEntry.range);
  };

  return (
    <div style={{ padding: "20px", minHeight: "100vh" }}>
      <h1 style={{ textAlign: "center" }}>Real-Time Data Visualization</h1>
      <h2 style={{ textAlign: "center" }}>WebSocket with MQTT - Dashboard</h2>
      <h3 style={{ textAlign: "center" }}>REAI CMU: Manatee</h3>

      <div style={{ display: "flex", justifyContent: "center" }}>
        <div style={{ width: "75%", maxWidth: "900px" }}>
          <p style={{ textAlign: "center" }}>Status: {connectionStatus}</p>

          <div style={{ textAlign: "center", marginBottom: "10px" }}>
            <Button variant="contained" color="primary" onClick={handleStartStop}>
              {isPlotting ? "Stop Plotting" : "Start Plotting"}
            </Button>
          </div>

          <div style={{ textAlign: "center", marginBottom: "20px" }}>
            <Checkbox checked={activePlots.energy} onChange={handleCheckboxChange} name="energy" />
            Energy Consumption
            <Checkbox checked={activePlots.pressure} onChange={handleCheckboxChange} name="pressure" style={{ marginLeft: "20px" }} />
            Pressure
            <Checkbox checked={activePlots.force} onChange={handleCheckboxChange} name="force" style={{ marginLeft: "20px" }} />
            Force
            <Checkbox checked={activePlots.position} onChange={handleCheckboxChange} name="position" style={{ marginLeft: "20px" }} />
            Position of Punch
          </div>

          {/* Main Graph */}
          <div style={{ boxShadow: "0px 4px 12px rgba(0, 0, 0, 0.1)", padding: "20px", borderRadius: "10px", background: "#fff", marginBottom: "20px" }}>
            <ResponsiveContainer width="100%" height={400}>
              <LineChart data={data} margin={{ top: 20, right: 30, left: 20, bottom: 0 }}>
                <CartesianGrid strokeDasharray="3 3" stroke="#f5f5f5" />
                <XAxis dataKey="timestamp" tick={{ fontSize: 12 }} />
                <YAxis tick={{ fontSize: 12 }} />
                <Tooltip contentStyle={{ backgroundColor: "#333", color: "#fff", borderRadius: "5px" }} />
                <Legend verticalAlign="top" height={36} iconType="circle" />
                <Brush dataKey="timestamp" height={20} stroke="#8884d8" />

                {activePlots.energy && (
                  <Line type="monotone" dataKey="energy" stroke="#8884d8" name="Energy Consumption (W)" strokeWidth={2} dot={false} />
                )}
                {activePlots.pressure && (
                  <Line type="monotone" dataKey="pressure" stroke="#82ca9d" name="Pressure (Pa)" strokeWidth={2} dot={false} />
                )}
                {activePlots.force && (
                  <Line type="monotone" dataKey="force" stroke="#ffc658" name="Force (N)" strokeWidth={2} dot={false} />
                )}
                {activePlots.position && (
                  <Line type="monotone" dataKey="position" stroke="#ff7300" name="Position (mm)" strokeWidth={2} dot={false} />
                )}
              </LineChart>
            </ResponsiveContainer>
          </div>

          {/* Selected Log Data Graph */}
          {selectedLogData && (
            <div style={{ boxShadow: "0px 4px 12px rgba(0, 0, 0, 0.1)", padding: "20px", borderRadius: "10px", background: "#fff", marginTop: "20px" }}>
              <h3 style={{ textAlign: "center" }}>Selected Data Range: {selectedRange}</h3>
              <ResponsiveContainer width="100%" height={300}>
                <LineChart data={selectedLogData} margin={{ top: 20, right: 30, left: 20, bottom: 0 }}>
                  <CartesianGrid strokeDasharray="3 3" stroke="#f5f5f5" />
                  <XAxis dataKey="timestamp" tick={{ fontSize: 12 }} />
                  <YAxis tick={{ fontSize: 12 }} />
                  <Tooltip />
                  <Legend />
                  <Brush dataKey="timestamp" height={20} stroke="#8884d8" />

                  {activePlots.energy && (
                    <Line type="monotone" dataKey="energy" stroke="#8884d8" name="Energy Consumption (W)" strokeWidth={2} dot={false} />
                  )}
                  {activePlots.pressure && (
                    <Line type="monotone" dataKey="pressure" stroke="#82ca9d" name="Pressure (Pa)" strokeWidth={2} dot={false} />
                  )}
                  {activePlots.force && (
                    <Line type="monotone" dataKey="force" stroke="#ffc658" name="Force (N)" strokeWidth={2} dot={false} />
                  )}
                  {activePlots.position && (
                    <Line type="monotone" dataKey="position" stroke="#ff7300" name="Position (mm)" strokeWidth={2} dot={false} />
                  )}
                </LineChart>
              </ResponsiveContainer>
            </div>
          )}
        </div>

        {/* Received message display */}
        <div style={{ textAlign: "center", marginTop: "20px" }}>
          <p>Received Message: {receivedMessage}</p>
        </div>

        {/* Data Log Section */}
        <div style={{ width: "25%", marginLeft: "20px" }}>
          <h3 style={{ textAlign: "center", marginBottom: "10px" }}>Data Logs</h3>
          <div style={{ boxShadow: "0px 4px 12px rgba(0, 0, 0, 0.1)", padding: "10px", borderRadius: "10px", background: "#fff", maxHeight: "400px", overflowY: "auto" }}>
            {dataLogs.map((logEntry) => (
              <p key={logEntry.id} style={{ cursor: "pointer", textAlign: "center" }} onClick={() => handleLogClick(logEntry)}>
                {logEntry.message || logEntry.range}
              </p>
            ))}
          </div>


          {/*Details*/}
          <div style={{textAlign: "center" , marginTop : "20px"}}>
            <b>Prediction: {data.PredictedLabel}</b>
            <b>Position: {data.Position}</b>
          </div>

          {/* Request Voice File Section */}
          <div style={{ textAlign: "center", marginTop: "20px" }}>
            {/* Request Voice File Button */}
            <Button variant="contained" color="primary" onClick={handleSend}>
              Request Voice File
            </Button>

            {/* Start and Stop buttons */}
            <div style={{ marginTop: "10px" }}>
              <Button 
                variant="contained" 
                color="secondary" 
                onClick={handleStart} 
                style={{ marginRight: "10px" }}
              >
                Start
              </Button>
              <Button 
                variant="contained" 
                color="secondary" 
                onClick={handleStop}
                style={{ marginRight: "10px" }}
              >
                Stop
              </Button>
              <Button 
                variant="contained" 
                color="secondary" 
                onClick={handleTest}
                style={{ marginRight: "10px" }}
              >
                Download MATLAB
              </Button>
            </div>

            {message && <p style={{ color: "#666", fontSize: "0.9em", marginTop: "5px" }}>{message}</p>}
          </div>
        </div>
      </div>
    </div>
  );
};

export default App;
