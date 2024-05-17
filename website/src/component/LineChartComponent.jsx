// LineChart.js
import React from "react";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
} from "recharts";

const LineChartComponent = ({ data }) => {
  return (
    <LineChart
      width={375}
      height={250}
      data={data}
      margin={{ top: 20, right: 30, left: 10, bottom: 10 }}
    >
      {/* <CartesianGrid strokeDasharray="3 3" /> */}
      <XAxis dataKey="name" />
      <YAxis />
      {/* <Tooltip /> */}
      <Legend />
      <Line
        type="monotone"
        dataKey="dust"
        stroke="#78716c"
        activeDot={{ r: 8 }}
      />
      <Line
        type="monotone"
        dataKey="humidity"
        stroke="#0284c7"
        activeDot={{ r: 8 }}
      />
      <Line
        type="monotone"
        dataKey="temp"
        stroke="#b91c1c"
        activeDot={{ r: 8 }}
      />
    </LineChart>
  );
};

export default LineChartComponent;
