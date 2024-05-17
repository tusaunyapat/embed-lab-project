import React from "react";
import LineChartComponent from "./LineChartComponent";
import { data } from "autoprefixer";
import { useEffect } from "react";

const Uselinegraprh = ({ dataArray }) => {
  // Example data (replace with your actual data)
  useEffect(() => {
    // Log dataArray when it's not empty
    if (dataArray.length > 0) {
      console.log("dataArray contains elements:", dataArray);
    }
  }, [dataArray]);

  return (
    <div className="bg-slate-100 p-2 rounded-xl">
      <LineChartComponent data={dataArray} />
    </div>
  );
};

export default Uselinegraprh;
