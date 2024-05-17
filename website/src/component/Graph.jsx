import React, { useState, useEffect } from "react";

function Detail({ data }) {
  // Initialize state to store humidity values
  const [humidityArray, setHumidityArray] = useState([]);

  // Update humidityArray when data prop changes
  useEffect(() => {
    if (data && data.humidity) {
      // Append new humidity value to the existing array
      setHumidityArray((prevHumidityArray) => [
        ...prevHumidityArray,
        data.humidity,
      ]);
    }
  }, [data]); // Trigger effect when data prop changes

  if (!data) {
    return <p>No data available</p>;
  }

  const { humidity, temp, dust } = data;

  return (
    <div className="flex flex-col">
      <h1>Humidity Graph:</h1>
      <div>
        {/* Render humidity values dynamically */}
        {humidityArray.map((humid, index) => (
          <p key={index}>
            Humidity {index + 1}: {humid}
          </p>
        ))}
      </div>
      <p>Current Humidity: {humidity}</p>
      <p>Temperature: {temp}</p>
      <p>Dust Level: {dust}</p>
    </div>
  );
}

export default Detail;
